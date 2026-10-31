#include <assert.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <net/if.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "config.h"
#include "log.h"

#ifndef LUA_CONFIG_TOPLEVEL_TABLE_NAME
#define LUA_CONFIG_TOPLEVEL_TABLE_NAME "drp"
#endif

// Accessed via [drp_config_get]
static DrpConfig globalConfig;

const DrpConfig *drp_config_get(void) {
  return &globalConfig;
}

// TODO: Handle IPv6 and non-UDP DNS requests
// Assumes [handler->ip] is allocated.
// Return NULL on error.
const Handler *drp_config_lua_get_handler(const DrpConfig * config, Handler *const handler, const char *domain) {
  lua_State *L = config->state;

  lua_getglobal(L, "drp");
  lua_getfield(L, -1, "handler");

  if (!lua_isfunction(L, -1)) {
    LOG_ERROR("drp.handler is not a function\n");
    return nullptr;
  }
  
  lua_pushstring(L, domain);
  if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
    LOG_ERROR("Error in calling a handler: %s\n", lua_tostring(L, -1));
    return nullptr;
  }

  if (!lua_istable(L, -1)) {
    LOG_ERROR("drp.handler returned a non-table");
    return nullptr;
  }

  const char *strPtr = nullptr;

  lua_getfield(L, -1, "ifname");
  if (lua_isstring(L, -1)) {
    strPtr = lua_tostring(L, -1);
    strncpy(handler->ifname, strPtr, sizeof(handler->ifname));
  }
  lua_pop(L, 1);

  lua_getfield(L, -1, "ipv4");
  if (lua_isstring(L, -1)) {
    strPtr = lua_tostring(L, -1);
    strcpy(handler->ip, strPtr);
  }
  lua_pop(L, 1);
  

  return handler;
}


int drp_config_load(const char *filename) {
  LOG_PRINT("Creating new Lua state\n");
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  //luaopen_math(L);
  //luaopen_io(L);
  
  
  lua_newtable(L);
  lua_setglobal(L, "drp");

  LOG_PRINT("Loading Lua config\n");
  if (luaL_dofile(L, filename) != LUA_OK) {
    LOG_ERROR("Failed to load lua config: %s\n", lua_tostring(L, -1));
    lua_close(L);
    return -1;
  }

  lua_pop(L, lua_gettop(L));
  LOG_PRINT("Loaded lua config\n");


  globalConfig.state = L;
  return 0;
}

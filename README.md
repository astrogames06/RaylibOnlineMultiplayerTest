# RaylibOnlineMultiplayerTest

# SUMMARY OF WHAT IT IS GENERATED WITH AI

## Overview

This project demonstrates a multiplayer web game using **raylib**, **C++**, **Emscripten**, and **WebSockets** via JavaScript.

Players can move around using `W`, `A`, `S`, `D`, and each client updates other players’ positions in real time.

---

## C++ (Client) Code

* Defined a `Player` struct to store player position and color.
* Used `std::map<int, Player>` to keep track of all players.
* Created `init_websocket(url)` to connect to the server using `emscripten::val`.
* `send_position(x, y)` sends the local player's position to the server.
* `update_player(id, x, y)` updates or adds other players.
* Game loop uses `UpdateDrawFrame()` to:

  * Move the local player using keyboard input.
  * Send position updates.
  * Draw all players on screen.
* Used `emscripten_set_main_loop()` for Web target.

## JavaScript / HTML (Web Client)

* Defined `Module.onmessage_callback` to handle incoming messages from the WebSocket.
* Messages could be either **string** or **Blob**, so we used `FileReader` for `Blob` messages.
* Parsed the message with `JSON.parse()`.
* Ignored messages from self using `Module.player_id`.
* Called `Module.update_player(id, x, y)` for other players.

## WebSocket Server (Bun / Node.js)

* Simple WebSocket server using `ws` package.
* Broadcasts messages from a client to all other clients.
* Logs client connections and disconnections.

## Key Challenges Solved

1. **Blob Messages:** Converted incoming `Blob` messages to string before parsing JSON.
2. **Duplicate Updates:** Ignored messages from the local client to prevent overwriting local position.
3. **Player ID Generation:** Used `GetRandomValue()` in C++ for unique player IDs.
4. **Continuous Updates:** Client always sends position each frame, even if not moving, for reliable multiplayer sync.

## Lessons Learned

* Handling different WebSocket data types (`Blob` vs string) is essential for WebAssembly clients.
* Always ignore self messages when broadcasting in multiplayer games.
* Random IDs for players prevent conflicts.
* Proper `Module` bindings are required to call C++ functions from JavaScript.

---

### References

* [Raylib Docs](https://www.raylib.com/)
* [Emscripten WebSockets](https://emscripten.org/docs/porting/networking/websockets.html)
* [WebSocket API MDN](https://developer.mozilla.org/en-US/docs/Web/API/WebSocket)
* [Bun WebSocket Docs](https://bun.sh/docs/api/ws)

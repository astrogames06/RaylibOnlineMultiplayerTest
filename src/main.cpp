#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#define PLATFORM_WEB
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
	#include <emscripten/bind.h>
	#include <emscripten/val.h>
#endif

const int WIDTH = 850;
const int HEIGHT = 450;

struct Player
{
	int x;
	int y;
	emscripten::val color;
};
std::map<int, Player> players;
int player_id;

emscripten::val websocket = emscripten::val::undefined();

void init_websocket(std::string url)
{
	websocket = emscripten::val::global("WebSocket").new_(url);

	websocket.set("onopen", emscripten::val::module_property("onopen_callback"));
	websocket.set("onclose", emscripten::val::module_property("onclose_callback"));

	websocket.set("onmessage", emscripten::val::module_property("onmessage_callback"));
}

void send_position(float x, float y)
{
	if (websocket.isUndefined()) return;
	if (websocket["readyState"].as<int>() != 1) return;

	emscripten::val msg = emscripten::val::object();
	msg.set("id", player_id);
	msg.set("x", x);
	msg.set("y", y);

	websocket.call<void>("send", emscripten::val::global("JSON").call<emscripten::val>("stringify", msg));
}

float player_x, player_y = 0;
void update_player(int id, int x, int y) {
    Player p;
    p.x = x;
    p.y = y;
    p.color = (id == player_id) ? emscripten::val::global("RED") : emscripten::val::global("BLUE");
    players[id] = p;

	emscripten::val::global("console").call<void>(
		"log", 
		"Player " + std::to_string(player_id) + " pos: (" + 
		std::to_string(player_x) + "," + std::to_string(player_y) + ")"
	);
}

void UpdateDrawFrame();

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window");
	player_id = GetRandomValue(1, 10000);

	#if defined(PLATFORM_WEB)
    	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
	#else
		SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
		//--------------------------------------------------------------------------------------

		// Main game loop
		while (!WindowShouldClose())    // Detect window close button or ESC key
		{
			UpdateDrawFrame();
		}
	#endif

	CloseWindow();

	return 0;
}

void UpdateDrawFrame()
{
	if (IsKeyDown(KEY_W)) player_y -= 1;
	if (IsKeyDown(KEY_A)) player_x -= 1;
	if (IsKeyDown(KEY_S)) player_y += 1;
	if (IsKeyDown(KEY_D)) player_x += 1;

	send_position(player_x, player_y);
	Player p;
	p.x = player_x;
	p.y = player_y;
	p.color = emscripten::val::global("RED");

	players[player_id] = p;

	BeginDrawing();
	
	ClearBackground(RAYWHITE);

	for (std::pair<const int, Player>& player : players)
	{
		Color c = RED;
		if (player.first != player_id) c = BLUE;
		DrawRectangle(player.second.x, player.second.y, 50, 50, c);
	}

	EndDrawing();
}


EMSCRIPTEN_BINDINGS(main_module) {
    emscripten::function("init_websocket", &init_websocket);
    emscripten::function("update_player", &update_player);
}
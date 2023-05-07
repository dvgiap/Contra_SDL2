#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerPower.h"
#include "Geometric.h"
#include <fstream>

fstream score_file;

BaseObject g_background;
TTF_Font* font_time = NULL;
TTF_Font* g_font_MENU = NULL;

bool InitData()
{
	bool success = true;

	g_window = SDL_CreateWindow("SDL2.0 Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
	if (g_window == NULL || g_screen == NULL) success = false;

	SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) && imgFlags)) success = false;

	if (TTF_Init() == -1) success = false;

	font_time = TTF_OpenFont("font/dlxfont_.ttf", 15);
	if (font_time == NULL) success = false;

	g_font_MENU = TTF_OpenFont("font/dlxfont_.ttf", 80);
	if (g_font_MENU == NULL) success = false;

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) success = false;

	// read files wav audio
	g_music = Mix_LoadMUS("sound/music.mp3");
	g_sound_bullet[0] = Mix_LoadWAV("sound/Gun+Silencer.wav");
	g_sound_bullet[1] = Mix_LoadWAV("sound/Gun+Shot2.wav");
	g_sound_explosion[0] = Mix_LoadWAV("sound/Gun+357+Magnum.wav");
	g_sound_jump = Mix_LoadWAV("sound/cartoon-jump-6462.wav");

	if (g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL || g_sound_explosion == NULL || g_music == NULL) success = false;

	return success;
}

vector<GameMap*> map_list;
bool LoadMap()
{
	bool ret = true;
	GameMap* map1 = new GameMap();
	map1->LoadMap("map//map1//map.dat");
	map_list.push_back(map1);

	GameMap* map2 = new GameMap();
	map2->LoadMap("map//map2//map.dat");
	map_list.push_back(map2);

	GameMap* map3 = new GameMap();
	map3->LoadMap("map//map3//map.dat");
	map_list.push_back(map3);

	GameMap* map4 = new GameMap();
	map4->LoadMap("map//map4//map.dat");
	map_list.push_back(map4);

	GameMap* map5 = new GameMap();
	map5->LoadMap("map//map5//map.dat");
	map_list.push_back(map5);

	GameMap* map6 = new GameMap();
	map6->LoadMap("map//map6//map.dat");
	map_list.push_back(map6);

	if (map_list.size() < TOTAL_MAP)
	{
		ret = false;
		cout << "fail to load map list \n";
	}
	return ret;
}

bool LoadBackground()
{
	return g_background.LoadImg("images/background.png", g_screen);
}

void close()
{
	for (auto map : map_list)
	{
		delete map;
	}
	map_list.clear();

	g_background.Free();

	Mix_FreeChunk(g_sound_bullet[0]);
	Mix_FreeChunk(g_sound_bullet[1]);
	Mix_FreeChunk(g_sound_explosion[0]);
	Mix_FreeChunk(g_sound_explosion[1]);
	Mix_FreeChunk(g_sound_jump);
	Mix_FreeMusic(g_music);
	TTF_CloseFont(font_time);
	TTF_CloseFont(g_font_MENU);
	g_sound_bullet[0] = NULL;
	g_sound_bullet[1] = NULL;
	g_sound_explosion[0] = NULL;
	g_sound_explosion[1] = NULL;
	g_sound_jump = NULL;
	g_music = NULL;

	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;
	SDL_DestroyWindow(g_window);
	g_window = NULL;

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

vector<ThreatsObject*> MakeThreatList()
{
	vector<ThreatsObject*> list_threats;

	ThreatsObject* dynamic_threats = new ThreatsObject[30];

	for (int i = 0; i < 30; i++)
	{
		ThreatsObject* p_threat = dynamic_threats + i;
		if (p_threat != NULL)
		{
			p_threat->LoadImg("images/thread_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);

			p_threat->set_x_pos(1500 + i * 800);
			p_threat->set_y_pos(200);

			int pos1 = p_threat->get_x_pos() - 100;
			int pos2 = p_threat->get_x_pos() + 100;
			p_threat->SetAnimationPos(pos1, pos2);
			p_threat->set_input_left(1);

			list_threats.push_back(p_threat);
		}
	}

	ThreatsObject* threats_objs = new ThreatsObject[20];

	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = threats_objs + i;
		if (p_threat != NULL)
		{
			p_threat->LoadImg("images/threats1.png", g_screen);
			p_threat->set_clips();
			p_threat->set_x_pos(700 + i*1200);
			p_threat->set_y_pos(250);
			p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p_threat->set_input_left(0);

			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);

			list_threats.push_back(p_threat);
		}
	}

	return list_threats;
}


int main(int argc, char* argv[])
{
	ImpTimer fps_timer;
	if (!InitData() || !LoadBackground() || !LoadMap()) return -1;

again_label:
	Mix_PlayMusic(g_music, -1);
	int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU, "Play Game", "Exit", "images//MENU.png");
	if (ret_menu == 1) return -1;
	
	PlayerPower player_power;
	player_power.Init(g_screen);
	PlayerMoney player_money;
	player_money.Init(g_screen);
	player_money.SetPos(SCREEN_WIDTH * 0.5 - 300, 4);
	TextObject time_game;
	time_game.SetColor(TextObject::WHITE_TEXT);
	TextObject mark_game;
	mark_game.SetColor(TextObject::WHITE_TEXT);
	UINT mark_value = 0;
	TextObject money_game;
	money_game.SetColor(TextObject::WHITE_TEXT);
	TextObject highest_score;

	int num_die = 0;
	int money_count = 0;
	
	MainObject p_player;
again_label1:
	// random map
	srand(time(0));
	int rand_map = rand() % TOTAL_MAP;
	GameMap* game_map = map_list.at(rand_map);
	game_map->LoadTiles(g_screen, rand_map + 1);
	
	p_player.set_start_pos();
	p_player.LoadImg("images/player_right.png", g_screen);
	p_player.set_clips();

	vector<ThreatsObject*> threats_list = MakeThreatList();

	ExplosionObject exp_threat;
	bool tRet = exp_threat.LoadImg("images/ex1.png", g_screen);
	if (!tRet)
	{
		cout << "could not load img ex1\n";
		return -1;
	}
	exp_threat.set_clip();

	ExplosionObject exp_main;
	bool pRet = exp_main.LoadImg("images//ex3.png", g_screen);
	if (!pRet)
	{
		cout << "could not load img explosion3\n";
		return -1;
	}
	exp_main.set_clip();

	bool is_quit = false;

	while (!is_quit)
	{
		fps_timer.start();
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}

			p_player.HandleInputAction(g_event, g_screen, g_sound_bullet, g_sound_jump);
		}

		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);

		g_background.Render(g_screen, NULL);
		//game_map->DrawMap(g_screen);

		Map map_data = game_map->getMap();

		p_player.HandleBullet(g_screen);
		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data);
		p_player.Show(g_screen);

		game_map->SetMap(map_data);
		game_map->DrawMap(g_screen);

		GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
		ColorData color_data(0, 80, 150);
		Geometric::RenderRectangle(rectangle_size, color_data, g_screen);
		GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);
		ColorData color_data2(255, 255, 255);
		Geometric::RenderOutline(outLineSize, color_data2, g_screen);

		player_power.Show(g_screen);
		player_money.Show(g_screen);

		for (int i = 0; i < threats_list.size(); i++)
		{
			ThreatsObject* p_threat = threats_list.at(i);
			if (p_threat != NULL)
			{
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->ImpMoveType(g_screen, map_data);
				p_threat->DoPlayer(map_data);
				p_threat->Show(g_screen);
				SDL_Rect rect_player = p_player.GetRectFrame();

				float player_x_pos = p_player.get_x_pos();

				if (player_x_pos <= map_data.max_x_ && player_x_pos >= map_data.max_x_ - 128)
				{
					// di het map
					SDL_Delay(500);
					goto again_label1;

				}
				bool bCol1 = false;
				SDL_Rect rect_threat = p_threat->GetRectFrame();
				int dis = rect_threat.x - rect_player.x;
				if (dis < 500 && dis >= 0)
				{
					p_threat->MakeBullet(g_screen, SCREEN_WIDTH);
				}
				std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
				
				for (int jj = 0; jj < tBullet_list.size(); ++jj)
				{
					BulletObject* pt_bullet = tBullet_list.at(jj);
					if (pt_bullet)
					{
						bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
						if (bCol1)
						{
							p_threat->RemoveBullet(jj);
							break;
						}
					}
				}

				rect_threat = p_threat->GetRectFrame();
				bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);

				if (bCol1 || bCol2)
				{
					Mix_PlayChannel(-1, g_sound_explosion[0], 0);
					int width_exp_frame = exp_main.get_frame_width();
					int height_exp_frame = exp_main.get_frame_height();
					for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
					{
						int x_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - width_exp_frame * 0.5;
						int y_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - height_exp_frame * 0.5;

						exp_main.set_frame(ex);
						exp_main.SetRect(x_pos, y_pos);
						exp_main.Show(g_screen);
						SDL_RenderPresent(g_screen);
					}
					num_die++;
					if (num_die <= 3)
					{
						p_player.SetRect(0, 0);
						p_player.set_comeback_time(60);
						SDL_Delay(1000);
						player_power.Decrease();
						player_power.Render(g_screen);
						continue;
					}
					else
					{
						score_file.open("highscore.txt", ios::app);
						score_file << p_player.GetMark() << " ";
						score_file.close();
						Sleep(500);
						int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU,
							"Play Again", "Exit",
							"img//MENU END.png");
						if (ret_menu == 1)
						{
							is_quit = true;
							continue;
						}
						else
						{
							is_quit = false;
							goto again_label;
						}
						p_threat->Free();
						close();
						SDL_Quit();
						return 0;
					}
				}
			}
		}


		int frame_exp_width = exp_threat.get_frame_width();
		int frame_exp_height = exp_threat.get_frame_height();

		vector <BulletObject*> bullet_arr = p_player.get_bullet_list();

		for (int r = 0; r < bullet_arr.size(); r++)
		{
			BulletObject* p_bullet = bullet_arr.at(r);
			if (p_bullet != NULL)
			{
				for (int t = 0; t < threats_list.size(); t++)
				{
					ThreatsObject* obj_threat = threats_list.at(t);
					if (obj_threat != NULL)
					{
						SDL_Rect tRect; // applied for threats
						tRect.x = obj_threat->GetRect().x;
						tRect.y = obj_threat->GetRect().y;
						tRect.w = obj_threat->get_width_frame();
						tRect.h = obj_threat->get_height_frame();

						SDL_Rect bRect = p_bullet->GetRect(); // applied for bullet (bullet has no frame clip)

						bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);
						if (bCol)
						{
							Mix_PlayChannel(-1, g_sound_explosion[0], 0);
							mark_value++;
							p_player.set_mark_val(mark_value);
							for (int ex = 0; ex < NUM_FRAME_EXP; ++ex)
							{
								int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
								int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;

								exp_threat.set_frame(ex);
								exp_threat.SetRect(x_pos, y_pos);
								exp_threat.Show(g_screen);
							}


							p_player.RemoveBullet(r);
							obj_threat->Free();
							threats_list.erase(threats_list.begin() + t);
						}
					}
				}
			}
		}

		//Show game time
		string str_time = "Time: ";
		Uint32 time_val = SDL_GetTicks() / 1000;
		Uint32 val_time = 600 - time_val;

		if (val_time <= 0)
		{
			Sleep(500);
			int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU,
				"Play Again", "Exit",
				"img//MENU END.png");
			if (ret_menu == 1)
			{
				is_quit = true;
				continue;
			}
		}
		else
		{
			string str_val = to_string(val_time);
			str_time += str_val;

			time_game.SetText(str_time);
			time_game.LoadFromRenderText(font_time, g_screen);
			time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
		}
		// Show mark
		string val_str_mark = to_string(mark_value);
		string strMark("Mark: ");
		strMark += val_str_mark;

		mark_game.SetText(strMark);
		mark_game.LoadFromRenderText(font_time, g_screen);
		mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);

		// Show highest score
		string strHighScore("High score: ");
		
		score_file.open("highscore.txt");
		if (!score_file) return -1;
		int score_in_file;
		int max_score = -1;
		while(score_file >> score_in_file)
			if ( score_in_file > max_score) max_score = score_in_file;
		// ra max trong file la bao nhieu
		score_file.close();
		int p_score = p_player.GetMark();
		if (p_score > max_score) max_score = p_score;
		strHighScore += to_string(max_score);
		highest_score.SetText(strHighScore);
		highest_score.LoadFromRenderText(font_time, g_screen);
		highest_score.RenderText(g_screen, SCREEN_WIDTH * 0.5 + 150, 15);

		// hien money
		money_count = p_player.GetMoneyCount();
		if (money_count >= 40)
		{
			player_power.Increase();
			player_power.Render(g_screen);
			p_player.SetMoneyCount(0);
			num_die--;
		}
		string money_str = to_string(money_count);
		money_game.SetText(money_str);
		money_game.LoadFromRenderText(font_time, g_screen);
		money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 15);

		SDL_RenderPresent(g_screen);

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND; // (ms)

		if (real_imp_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0) SDL_Delay(delay_time);
		}
	}

	for (int i = 0; i < threats_list.size(); i++)
	{
		ThreatsObject* p_threat = threats_list.at(i);
		if (p_threat != NULL)
		{
			p_threat->Free();
			p_threat = NULL;
		}
	}

	close();
	return 0;
}

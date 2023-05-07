﻿#ifndef MAIN_OBJECT_H
#define MAIN_OBJECT_H

#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 10
#define PLAYER_JUMP_VAL 20

class MainObject : public BaseObject
{
public:
	MainObject();
	~MainObject();

	enum WalkType
	{
		WALK_NONE = 0,
		WALK_RIGHT = 0,
		WALK_LEFT = 1,
	};

	float get_x_pos() const { return x_pos_; }
	float get_y_pos() const { return y_pos_; }
	bool LoadImg(string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound[2], Mix_Chunk* jump_sound);
	void set_clips();
	void DoPlayer(Map& map_data); // xử lý tốc độ di chuyển, rơi...
	void CheckToMap(Map& map_data);
	void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }
	void CenterEntityOnMap(Map& map_data);
	void UpdateImagePlayer(SDL_Renderer* des);
	SDL_Rect GetRectFrame();
	void set_bullet_list(vector<BulletObject*> bullet_list) { p_bullet_list_ = bullet_list; }
	vector<BulletObject*> get_bullet_list() const { return p_bullet_list_; }
	void HandleBullet(SDL_Renderer* des);
	void RemoveBullet(const int& idx);
	void IncreaseMoney();
	int get_frame_width() const { return width_frame_; }
	int get_frame_height() const { return height_frame_; }
	void set_comeback_time(const int& cb_time) { come_back_time_ = cb_time; }
	int GetMoneyCount() const { return money_count; }
	void SetMoneyCount(const int& money) { money_count = money; }
	int GetMark() const { return mark_; }
	void set_mark_val(const int& mark_val) { mark_ = mark_val; }
	void set_start_pos() { x_pos_ = 0; y_pos_ = 0; }

private:

	int money_count;

	vector<BulletObject*> p_bullet_list_;
	float x_val_; // val tăng khi ấn sang phải
	float y_val_; // tạm thời chưa cần
	float x_pos_; // vị trí hiện tại
	float y_pos_; // vị trí hiện tại
	int width_frame_; // kích thước 1 frame
	int height_frame_; // kích thước 1 frame

	SDL_Rect frame_clip_[8];
	Input input_type_;
	int frame_; // lưu chỉ số frame 0 -> 7
	int status_; // trạng thái di chuyển trái hay phải
	bool on_ground_;

	int map_x_; // lượng mà bản đồ bị cuốn theo chiều ngang
	int map_y_;
	int come_back_time_;
	int mark_;
};

#endif // !MAIN_OBJECT_H



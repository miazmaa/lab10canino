#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <stdio.h>
#include "Sprite.h"

#include <iostream>
using namespace std;

void sprite::drawSprite()
{
	if (DeadSprite) {
		return;
	}
	else if (ScaredSprite && CollisionIsTrue && al_get_time() - collisionTime < 3.0) {
		al_draw_tinted_bitmap(image[curframe], color, x, y, 0);
	}
	else if (BabySprite && CollisionIsTrue && al_get_time() - collisionTime < 10.0) {
		al_draw_scaled_bitmap(image[curframe], 0, 0, width, height, x, y, width * scale, height * scale, 0);
	}
	else {
		al_draw_bitmap(image[curframe], x, y, 0);
	}
}

void sprite::updatesprite()
{
	//update x position
	if (++xcount > xdelay)
	{
		xcount = 0;
		x += xspeed;
	}

	//update y position
	if (++ycount > ydelay)
	{
		ycount = 0;
		y += yspeed;
	}


	if (framecount++ > framedelay)
	{
		framecount = 0;
		curframe++;
		if (curframe >= maxframe)
			curframe = 0;
	}
	if (CollisionIsTrue)
	{
		if (ScaredSprite && al_get_time() - collisionTime > 3.0)
			CollisionIsTrue = false;
		if (FreezeSprite && freeze && al_get_time() - collisionTime > 5.0)
		{
			xspeed = prevxspeed;
			yspeed = prevyspeed;

			freeze = false;
			CollisionIsTrue = false;
			x += xspeed;
			y += yspeed;
		}
		if (BabySprite && al_get_time() - collisionTime > 10.0)
			CollisionIsTrue = false;
	}
}

void sprite::bouncesprite(int SCREEN_W, int SCREEN_H)
{
	//simple screen bouncing behavior
	if (x < 0)
	{
		x = 0;
		xspeed = rand() % 2 + 4;
		animdir *= -1;
	}

	else if (x > SCREEN_W - width)
	{
		x = SCREEN_W - width;
		xspeed = rand() % 2 - 6;
		animdir *= -1;
	}

	if (y < 0)
	{
		y = 0;
		yspeed = rand() % 2 + 4;
		animdir *= -1;
	}

	else if (y > SCREEN_H - height)
	{
		y = SCREEN_H - height;
		yspeed = rand() % 2 - 6;
		animdir *= -1;
	}

}

void sprite::load_animated_sprite(int size)
{
	//load the animated sprite
	char s[80];
	maxframe = size;
	for (int n = 0; n < size; n++)
	{
		sprintf_s(s, "Alien%d.bmp", n);
		image[n] = al_load_bitmap(s);

		al_convert_mask_to_alpha(image[n], al_map_rgb(255, 255, 255));
	}
	width = al_get_bitmap_width(image[0]);
	height = al_get_bitmap_height(image[0]);
	curframe = 0;
	framedelay = 5;
	framecount = 0;
	color = al_map_rgb(255, 255, 255);
	CollisionIsTrue = false;
	DeadSprite = false; //only for baby
	SpinningSprite = false;
	ScaredSprite = false;
	BabySprite = false;
	FreezeSprite = false;
	xspeed = rand() % 5 + 1;
	yspeed = rand() % 5 + 1;
	x = rand() % 600;
	y = rand() % 440;
	xcount = 0;
	ycount = 0;
	prevxspeed = xspeed;
	prevyspeed = yspeed;
	freeze = false;

	animdir = 1;
	xdelay = 0;
	ydelay = 0;
	scale = 1.0f;
	int skill = rand() % 4; //for picking sprite type
	switch (skill)
	{
	case 0:
		ScaredSprite = true;
		break;

	case 1:
		BabySprite = true;
		break;

	case 2:
		SpinningSprite = true;
		break;

	case 3:
		FreezeSprite = true;
		cout << "Freeze sprite created\n";
		break;
	}
}

sprite::~sprite()
{
	for (int i = 0; i < maxframe; i++)
		al_destroy_bitmap(image[i]);
}
void sprite::Collision(sprite Sprites[], int cSize, int me, int WIDTH, int HEIGHT) {
	for (int i = 0; i < cSize; i++) {
		if (i != me) {
			if (x >= Sprites[i].getX() - width && x <= Sprites[i].getX() + width) {
				if (y >= Sprites[i].getY() - height && y <= Sprites[i].getY() + height) {
					if (!CollisionIsTrue)
					{
						CollisionIsTrue = true;
						collisionTime = al_get_time();
					}
					if (ScaredSprite) {
						color = al_map_rgb(rand() % 256, rand() % 256, rand() % 256);
						x = rand() % WIDTH;
						y = rand() % HEIGHT;
					}
					else if (BabySprite) {
						scale *= 0.5f;
						x = rand() % WIDTH;
						y = rand() % HEIGHT;
						if (scale <= 0.1f) {
							DeadSprite = true;
							cout << "Sprite is dead!";
						}
					}
					if (FreezeSprite && !freeze)
					{
						freeze = true;

						prevxspeed = xspeed;
						prevyspeed = yspeed;

						xspeed = 0;
						yspeed = 0;

						collisionTime = al_get_time();

						cout << "Sprite frozen!\n";
					}
				}
			}
		}
	}
}
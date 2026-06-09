#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "Sprite.h"

#include <iostream>
using namespace std;

void sprite::drawSprite()
{
	if (DeadSprite) {
		return;
	}
	int fx = (curframe % animationColumns) * frameWidth;
	int fy = (curframe / animationColumns) * frameHeight;
	if (ScaredSprite && CollisionIsTrue && al_get_time() - collisionTime < 3.0) {
		al_draw_tinted_scaled_bitmap(image,color,fx, fy,frameWidth, frameHeight,x, y,frameWidth * scale,frameHeight * scale,0);
	}
	else if (BabySprite && CollisionIsTrue && al_get_time() - collisionTime < 10.0) {
		al_draw_scaled_bitmap(image, fx, fy, frameWidth, frameHeight, x, y, frameWidth * scale, frameHeight * scale, 0);
	}
	else if (SpinningSprite) {
		float cx = frameWidth / 2.0f;
		float cy = frameHeight / 2.0f;
		al_draw_scaled_rotated_bitmap(frames[curframe],cx, cy,x + cx * scale,y + cy * scale,scale,scale,angle,0);
	}
	else {
		al_draw_scaled_bitmap(image, fx, fy, frameWidth, frameHeight, x, y, frameWidth * scale, frameHeight * scale, 0);
	}
}

void sprite::updatesprite()
{
	prevx = x;
	prevy = y;
	if (SpinningSprite) {
		angle += 0.05f; 
		if (angle > 2 * ALLEGRO_PI) {
			angle -= 2 * ALLEGRO_PI;
		}
	}
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
		if (SpinningSprite) {
			CollisionIsTrue = false;
		}
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
	image = al_load_bitmap("mega.png");
	if (image) { al_convert_mask_to_alpha(image,al_map_rgb(255, 255, 255));
	}
	frameWidth = al_get_bitmap_width(image) / 5;
	frameHeight = al_get_bitmap_height(image) / 2;
	animationColumns = 5;
	maxframe = 10;

	for (int i = 0; i < maxframe; i++)
	{
		int fx = (i % animationColumns) * frameWidth;
		int fy = (i / animationColumns) * frameHeight;

		frames[i] = al_create_sub_bitmap(image, fx, fy, frameWidth, frameHeight);
	}
	framedelay = 5;
	framecount = 0;
	color = al_map_rgb(255, 255, 255);
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
	angle = 0.0f;
	scale = 0.5f;                  
	width = frameWidth * scale;      
	height = frameHeight * scale;
	animdir = 1;
	xdelay = 0;
	ydelay = 0;
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
		break;
	}
}

sprite::~sprite()
{
	for (int i = 0; i < maxframe; i++)
		al_destroy_bitmap(frames[i]);
	if (image)
		al_destroy_bitmap(image);
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
						width = frameWidth * scale;
						height = frameHeight * scale;
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

					}
					else if (SpinningSprite) {
						x = prevx;
						y = prevy; //restores previous position on collision
						CollisionIsTrue = true;
						collisionTime = al_get_time();
					}
				}
			}
		}
	}
}
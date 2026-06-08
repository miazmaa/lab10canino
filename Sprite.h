class sprite
{
public:
	~sprite();
	void updatesprite();
	void bouncesprite(int SCREEN_W, int SCREEN_H);
	void load_animated_sprite(int size);
	void drawSprite();
	int getX() { return x; }
	int getY() { return y; }
	void Collision(sprite Sprites[], int cSize, int me, int WIDTH, int HEIGHT);

private:
	int x, y;
	int width, height;
	int xspeed, yspeed;
	int xdelay, ydelay;
	int xcount, ycount;
	int curframe, maxframe, animdir;
	int framecount, framedelay;
	ALLEGRO_BITMAP* image[9];
	ALLEGRO_COLOR color;
	bool CollisionIsTrue;
	bool SpinningSprite;
	bool ScaredSprite;
	bool BabySprite; //bottom 4 used to decide what the sprite is doing
	bool FreezeSprite;
	bool DeadSprite;
};
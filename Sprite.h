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
	int prevx, prevy;
	int width, height;
	int xspeed, yspeed;
	int prevxspeed, prevyspeed;
	int xdelay, ydelay;
	int xcount, ycount;
	int curframe, maxframe, animdir;
	int framecount, framedelay;
	ALLEGRO_BITMAP* image;
	ALLEGRO_COLOR color;
	ALLEGRO_BITMAP* frames[10];
	double collisionTime;
	float scale; //for baby
	float angle; //for spin
	bool CollisionIsTrue;
	bool SpinningSprite;
	bool freeze;
	bool ScaredSprite;
	bool BabySprite; //bottom 4 used to decide what the sprite is doing
	bool FreezeSprite;
	bool DeadSprite;
	int frameWidth;
	int frameHeight;
	int animationColumns;
};
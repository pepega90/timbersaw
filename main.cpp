#include <raylib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

enum Scene
{
    MENU = 1,
    PLAY,
};

struct Player

{
    Texture2D img;
    Rectangle frameRect;
    Vector2 pos;
    Rectangle rect;
    bool live{};

    /*
        idle = 2 - 3
        cut = 0 - 1
    */

    // cut anim
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 8;

    // idle anim
    int curIdle = 2;
    int fCounterIdle = 0;
    int fIdleSpeed = 5;

    // animation flag
    bool idle = false;
    bool cut = false;
    bool flip = false;

    void Animation()
    {
        if (cut)
        {
            framesCounter++;

            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                currentFrame++;

                if (currentFrame > 1)
                    currentFrame = 0;

                frameRect.x = (float)currentFrame * (float)img.width / 4;
            }
        }

        if (idle)
        {
            fCounterIdle++;

            if (fCounterIdle >= (60 / fIdleSpeed))
            {
                fCounterIdle = 0;
                curIdle++;

                if (curIdle > 3)
                    curIdle = 2;

                frameRect.x = (float)curIdle * (float)img.width / 4;
            }
        }
    }

    void Update()
    {
        Animation();

        // keyboard input
        if (IsKeyDown(KEY_RIGHT))
        {
            flip = true;
            pos.x = 230;
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            flip = false;
            pos.x = 20;
        }

        rect.x = pos.x + 50;
        rect.y = pos.y + 90;
        rect.width = 50;
        rect.height = 10;

        Draw();
    }

    void Draw()
    {
        DrawTextureRec(img, Rectangle{frameRect.x, frameRect.y, flip ? -frameRect.width : frameRect.width, frameRect.height}, pos, WHITE);
        // DrawRectangleLinesEx(rect, 2, GREEN);
    }
};

struct Pohon
{
    Texture2D img;
    Rectangle rect;
};

void InitPohon(std::vector<Pohon> &treeList, Texture2D tree)
{
    for (int i = 0; i < 7; i++)
    {
        Pohon p;
        p.img = tree;
        // 157
        // 60
        p.rect.x = 157;

        p.rect.y = -10 + i * (float)p.img.height * 0.3;
        p.rect.width = (float)p.img.width * 0.3;
        p.rect.height = (float)p.img.height * 0.3;
        treeList.push_back(p);
    }
}

int main()
{
    const int WIDTH = 400;
    const int HEIGHT = 600;
    InitWindow(WIDTH, HEIGHT, "Timber-Saw");
    InitAudioDevice();
    SetTargetFPS(60);

    // variabel untuk posisi mouse
    std::string mousePos{};

    // load background
    Texture2D bg = LoadTexture("texture/background.png");

    // load soundfx
    Sound cutTreeSfx = LoadSound("snd/cut.ogg");
    Sound deathSfx = LoadSound("snd/death.mp3");

    // load btn play
    Texture2D btnPlay = LoadTexture("texture/playbtn-sheet0.png");
    Rectangle btnRect = {155, HEIGHT / 2, btnPlay.width * 0.7f, btnPlay.height * 0.7f};

    // load player
    Image sprite = LoadImage("texture/man.png");
    Texture2D playerImg = LoadTextureFromImage(sprite);

    // load player rip
    Texture2D rip = LoadTexture("texture/rip.png");

    // player data
    float playerScaled = 0.3;
    Player player;
    player.live = true;
    playerImg.width *= playerScaled;
    playerImg.height *= playerScaled;
    player.img = playerImg;
    player.pos = {20, 400};
    player.frameRect = {0, 0, (float)sprite.width / 4 * playerScaled, (float)sprite.height * playerScaled};

    // load pohon
    Texture2D dasar = LoadTexture("texture/stump.png");
    Vector2 dasarPos = Vector2{150, 500};
    Texture2D tree = LoadTexture("texture/pohon.png");
    // Load ranting
    Texture2D ranting1 = LoadTexture("texture/branch1.png");
    Texture2D ranting2 = LoadTexture("texture/branch2.png");
    // list ranting
    Texture2D listRanting[3] = {tree, ranting1, ranting2};
    std::vector<Pohon> treeList;
    InitPohon(treeList, tree);

    // list potongan
    std::vector<Pohon> treeCut;

    // scene
    Scene currentScene = MENU;

    // game variable
    bool hapus{}; // variabel hapus ini ketika kita cut pohon, untuk membuat pohon ke bawah
    int rand{}; // untuk merandom ranting pohon yang muncul, bisa kiri/kanan
    int rot = 0; // ini rotasi dari pohon yang sudah di cut

    // score
    Rectangle bar = {105, 30, 200, 30};
    Rectangle outline = {105, 30, 200, 30};
    std::string scoreText{};
    int score{};

    while (!WindowShouldClose())
    {
        ClearBackground(BLACK);

        BeginDrawing();
        DrawTextureEx(bg, Vector2{}, 0.0, 0.38, WHITE);
        DrawTextureEx(dasar, dasarPos, 0.0, 0.3, WHITE);
        switch (currentScene)
        {
        case MENU:

            player.Update();
            // untuk nampilkan pohon di menu (karena baru ke load di Scene PLAY)
            for (unsigned int i = 0; i < 7; i++)
            {
                DrawTextureEx(tree, Vector2{157, static_cast<float>(-10 + i * tree.height * 0.3)}, 0.0, 0.3, WHITE);
            }
            DrawText("Timber\n  Saw", 123, 60, 50, YELLOW);
            DrawTextureEx(btnPlay, Vector2{btnRect.x, btnRect.y}, 0.0, 0.7, WHITE);
            DrawText("created by aji mustofa \n       @pepega90", 70, 530, 25, WHITE);

            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btnRect))
                currentScene = PLAY;
            break;
        case PLAY:
            if (player.live)
                while (treeList.size() < 7)
                {
                    Pohon p;
                    rand = GetRandomValue(0, 2);
                    p.img = listRanting[rand];
                    // 157
                    // 56.5
                    if (rand == 1)
                        p.rect.x = 56.5;
                    else
                        p.rect.x = 157;

                    p.rect.y = -50;
                    p.rect.width = (float)p.img.width * 0.3;
                    p.rect.height = (float)p.img.height * 0.3;

                    treeList.insert(treeList.begin(), p);
                }

            for (unsigned int i = 0; i < treeList.size(); i++)
            {

                if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) && !hapus && player.live)
                {
                    // fallPos.x = treeList[treeList.size() - 1].rect.x;
                    // fallTexture = treeList[treeList.size() - 1].img;
                    treeCut.push_back(treeList[treeList.size() - 1]);
                    treeList.pop_back();
                    hapus = true;
                    player.cut = true;
                    player.idle = false;
                    PlaySound(cutTreeSfx);
                    score += 1;
                    if (bar.width < 200)
                        bar.width += 5;
                }
                // jika hapus true, makan turunkan semua pohon ke bawah sebanyak 10 koordinat-y nya
                if (hapus)
                {
                    treeList[i].rect.y += 10;
                }
                // jika pohon yang sudah di potong lebih besari dari dasar pohon, maka stop turunkan pohonnya
                if (treeList[i].rect.y + treeList[i].rect.height >= dasarPos.y)
                {
                    player.cut = false;
                    hapus = false;
                    player.idle = true;
                }

                if (CheckCollisionRecs(player.rect, treeList[i].rect) || bar.width < 0)
                    player.live = false;

                DrawTextureEx(treeList[i].img, Vector2{treeList[i].rect.x, treeList[i].rect.y}, 0.0, 0.3, WHITE);

                // DrawRectangleLinesEx(treeList[i].rect, 2, RED);
            }

            // animasi tree cut
            rot += 5;
            for (unsigned int i = 0; i < treeCut.size(); i++)
            {
                DrawTextureEx(treeCut[i].img, Vector2{treeCut[i].rect.x, treeCut[i].rect.y}, rot, 0.3, WHITE);
                treeCut[i].rect.x += player.flip ? -10 : 10;
                treeCut[i].rect.y += 8;
            }

            // hapus pohon yang sudah dipotong
            auto removeTree = std::remove_if(treeCut.begin(), treeCut.end(), [&](Pohon t)
                                             { return t.rect.y > HEIGHT + 50 || t.rect.x > WIDTH + 100 || t.rect.x < -50; });
            if (removeTree != treeCut.end())
            {
                treeCut.erase(removeTree);
                rot = 0;
            }

            // untuk nutupin. pohon yang ketika di cut
            for (unsigned int i = 0; i < 7; i++)
            {
                DrawTextureEx(tree, Vector2{157, static_cast<float>(-10 + i * tree.height * 0.3)}, 0.0, 0.3, WHITE);
            }

            // draw energy
            if(player.live)
                bar.width -= 0.5;
            DrawRectangleRec(bar, RED);
            DrawRectangleLinesEx(outline, 3, YELLOW);

            // draw score
            scoreText = std::to_string(score);
            DrawText(scoreText.c_str(), WIDTH / 2 - 3, 80, 40, WHITE);

            // jika player mati, maka kita tampilkan batu nisan
            if (player.live)
                player.Update();
            else
                DrawTextureEx(rip, dasarPos, 0.0, 1.0, WHITE);

            // game over
            if (!player.live)
            {
                DrawText("Tekan \"R\" untuk restart", WIDTH / 2 - 140, 270, 25, WHITE);

                if (IsKeyDown(KEY_R) && !player.live)
                {
                    bar.width = 200;
                    score = 0;
                    player.live = true;
                    treeList.clear();
                    InitPohon(treeList, tree);
                }
            }

            break;
        }

        // draw mouse position
        // mousePos = "X = " + std::to_string(GetMouseX()) + ", Y = " + std::to_string(GetMouseY());
        // DrawText(mousePos.c_str(), GetMousePosition().x - 30, GetMousePosition().y + 20, 20, WHITE);
        EndDrawing();
    }
    UnloadTexture(dasar);
    UnloadTexture(bg);
    UnloadTexture(playerImg);
    UnloadTexture(tree);
    UnloadTexture(ranting1);
    UnloadTexture(ranting2);
    UnloadTexture(btnPlay);
    UnloadSound(deathSfx);
    UnloadSound(cutTreeSfx);
    CloseWindow();
    CloseAudioDevice();

    return 0;
}
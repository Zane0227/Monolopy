#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<Windows.h>
#include<iostream>
#include<sstream>

#define WINDOW_HEIGHT 25	//窗口高度
#define WINDOW_WIDTH 80		//窗口宽度
#define STAGE_HEIGHT 1308		//界面高度
#define STAGE_WIDTH 2500		//界面宽度
#define SCALE 0.6				//缩放倍数

using namespace sf;

const int height = STAGE_HEIGHT;
const int width = STAGE_WIDTH;

VideoMode mode = VideoMode::getDesktopMode();
Vector2i ViewPos;
sf::RenderWindow window(sf::VideoMode(width* SCALE, height* SCALE), L"来玩大富翁!");


int Volume = 50;
Font font;
Text text;

Vector2i Loadposition;	//记录鼠标坐标

//游戏状态定义
bool GameOver, GameStart, GameQuit,PicGameOver;	
bool IfBuy,IsBuy,HaveBuy;

//玩家定义
int GameNum;//玩家数量
int MoneyPlayerOne, MoneyPlayerTwo;//玩家金钱
bool Turn_Player1 = true;//轮到玩家1
bool Turn_Player2 = false;//轮到玩家2
bool IfNeedHelp;
Texture tPlayer1, tPlayer2, tPlayer3, tPlayer4;
Sprite spPlayer1, spPlayer2, spPlayer3, spPlayer4;//玩家头像

//图片\按钮定义
bool StartButton_Move, StartButton;//开始按键移动、点击
bool ReStartButton_Move;
Texture tLoadingPic, tGamingBK,tGameTest, tPlayButton_HighLight, tPlayButton, tPlayButton_Move,tGouMai,tJuJue,tExit,tReStart,tReStart1;
Sprite spLoadingPic, spGamingBK,spGameTest, spPlayButton_Move, spPlayButton, spPlayButton_Start,spGouMai,spJuJue,spExit,spReStart,spReStart1;//图片按钮图像素材
Texture tBlueBackGround, tYellowBackGround,tBigBlueBackGround,tBigYellowBackGround;
Sprite spBlueBackGround, spYellowBackGround,spBigBlueBackGround,spBigYellowBackGround;

//建筑定义
Texture tBuding1, tBuding2, tBuding3, tBuding4, tBuding5, tBuding6, tBuding7, tBuding8, tBuding9, tBuding10, 
tBuding11, tBuding12, tBuding13, tBuding14, tBuding15, tBuding16, tBuding17, tBuding18, tBuding19, tBuding20, tBuding21, tBuding22;

Sprite spBuding1, spBuding2, spBuding3, spBuding4, spBuding5, spBuding6, spBuding7, spBuding8, spBuding9, spBuding10, spBuding11, 
spBuding12, spBuding13, spBuding14, spBuding15, spBuding16, spBuding17, spBuding18, spBuding19, spBuding20, spBuding21, spBuding22;

int Belong1, Belong2, Belong3, Belong4, Belong5, Belong6, Belong7, Belong8, Belong9, Belong10, Belong11, Belong12, Belong13, Belong14, Belong15, Belong16, Belong17, Belong18, Belong19, Belong20, Belong21, Belong22;//建筑属于谁
int RantMoney;//租金
int CostMoney;//购买金钱

//骰子定义
Texture tTouZi, tTouZiPic, tTouZiPic1;
Sprite spTouZi, spTouZiPic, spTouZiPic1;//骰子图像
bool TouZi_Move, TouZi_Click,Roll_TouZi;//骰子移动、点击、旋转动画
int TouZiClick_Ready;//锁定骰子状态
int TouZi_Num,TouZi_Num1;//骰子点数
int TouZiCartoon[20];//骰子动画数组
int TouZiTime;//骰子变换动画次数
int TouZiRollTime;//记录骰子扔的次数来判断现在是哪个玩家扔骰子
sf::IntRect Broll;//骰子相应区域

//游戏区域记录
int LocationX[49], LocationY[49];//地图格子数记录
int L1,L2;//记录玩家当前位置
bool ChangeLocation = false;//记录是否需要移动位置
bool BeginChange = false; //骰子动画结束后开始移动位置
int Copy_TouZi_Num = 0;//记录格子移动格数
Texture tPlayerMoney1, tPlayerMoney2;
Sprite spPlayerMoney1, spPlayerMoney2;//玩家金钱信息记录

//音乐记录
sf::Music bkMusic;
sf::SoundBuffer ClickMusicBuffer,UseMoneyMusicBuffer;
sf::Sound ClickMusic,UseMoneyMusic;

void InitialInt()
{
	CostMoney = 0;
	RantMoney = 0;
	IfBuy = false;
	IsBuy = false;
	HaveBuy = true;
	PicGameOver = false;
	GameStart = false;
	GameOver = false;
	StartButton_Move = false;
	StartButton = false;
	TouZi_Move = false;
	TouZi_Click = false;
	GameNum = 2;//玩家数量
	TouZi_Num = 0;// 骰子点数
	TouZiTime = 0;
	Roll_TouZi = false;
	TouZiClick_Ready = true;
	ReStartButton_Move = false;
	IfNeedHelp = false;
	for (int i = 0; i < 20; i++)
	{
		TouZiCartoon[i] = rand() % 6;
	}//骰子动画点数定义

	MoneyPlayerOne = 10000;
	MoneyPlayerTwo = 10000;//初始金钱

		//建筑归属关系初始化
	Belong1 = 0, Belong2 = 0, Belong3 = 0, Belong4 = 0, Belong5 = 0, Belong6 = 0, Belong7 = 0, Belong8 = 0, Belong9 = 0, Belong10 = 0, Belong11 = 0, Belong12 = 0,
		Belong13 = 0, Belong14 = 0, Belong15 = 0, Belong16 = 0, Belong17 = 0, Belong18 = 0, Belong19 = 0, Belong20 = 0, Belong21 = 0, Belong22 = 0;
	//Belong=0时代表建筑无归属，=1代表属于1，=2代表属于2
	L1 = 1, L2 = 0;
	TouZiRollTime = 0;
}

void initial() 
{
	ViewPos.x = (mode.width - STAGE_WIDTH * SCALE) / 2;
	ViewPos.y = (mode.height - STAGE_HEIGHT * SCALE) / 2;
	window.setPosition(ViewPos);
	//区域信息记录
	LocationX[0] = 980;		LocationY[0] = 1150;
	LocationX[1] = 870;		LocationY[1] = 1150;
	LocationX[2] = 750;		LocationY[2] = 1150;
	LocationX[3] = 640;		LocationY[3] = 1150;
	LocationX[4] = 530;		LocationY[4] = 1150;
	LocationX[5] = 420;		LocationY[5] = 1150;
	LocationX[6] = 300;		LocationY[6] = 1150;
	LocationX[7] = 190;		LocationY[7] = 1150;
	LocationX[8] = 190;		LocationY[8] = 1030;
	LocationX[9] = 80;		LocationY[9] = 1020;
	LocationX[10] = 80;		LocationY[10] = 910;
	LocationX[11] = 80;		LocationY[11] = 800;
	LocationX[12] = 80;		LocationY[12] = 690;
	LocationX[13] = 90;		LocationY[13] = 590;
	LocationX[14] = 190;	LocationY[14] = 590;
	LocationX[15] = 300;	LocationY[15] = 590;
	LocationX[16] = 410;	LocationY[16] = 590;
	LocationX[17] = 420;	LocationY[17] = 470;
	LocationX[18] = 420;	LocationY[18] = 360;
	LocationX[19] = 420;	LocationY[19] = 250;
	LocationX[20] = 310;	LocationY[20] = 260;
	LocationX[21] = 210;	LocationY[21] = 200;
	LocationX[22] = 310;	LocationY[22] = 140;
	LocationX[23] = 420;	LocationY[23] = 140;
	LocationX[24] = 530;	LocationY[24] = 140;
	LocationX[25] = 640;	LocationY[25] = 140;
	LocationX[26] = 650;	LocationY[26] = 250;
	LocationX[27] = 650;	LocationY[27] = 360;
	LocationX[28] = 760;	LocationY[28] = 370;
	LocationX[29] = 870;	LocationY[29] = 370;
	LocationX[30] = 980;	LocationY[30] = 370;
	LocationX[31] = 990;	LocationY[31] = 250;
	LocationX[32] = 990;	LocationY[32] = 140;
	LocationX[33] = 1100;	LocationY[33] = 150;
	LocationX[34] = 1220;	LocationY[34] = 150;
	LocationX[35] = 1340;	LocationY[35] = 150;
	LocationX[36] = 1440;	LocationY[36] = 150;
	LocationX[37] = 1440;	LocationY[37] = 250;
	LocationX[38] = 1440;	LocationY[38] = 360;
	LocationX[39] = 1430;	LocationY[39] = 470;
	LocationX[40] = 1320;	LocationY[40] = 480;
	LocationX[41] = 1200;	LocationY[41] = 480;
	LocationX[42] = 1100;	LocationY[42] = 480;
	LocationX[43] = 1100;	LocationY[43] = 590;
	LocationX[44] = 1100;	LocationY[44] = 700;
	LocationX[45] = 1100;	LocationY[45] = 810;
	LocationX[46] = 980;	LocationY[46] = 820;
	LocationX[47] = 980;	LocationY[47] = 930;
	LocationX[48] = 980;	LocationY[48] = 1040;
	//区域信息记录结束
	if (!bkMusic.openFromFile("./things/audio/bkmusic.wav"))
	{
		std::cout << "背景音乐加载失败" << std::endl;
	}
	bkMusic.setLoop(true);
	bkMusic.play();
	if (!ClickMusicBuffer.loadFromFile("./things/audio/10010.wav"))
	{
		std::cout << "点击音乐加载失败" << std::endl;
	}
	ClickMusic.setBuffer(ClickMusicBuffer);
	if (!UseMoneyMusicBuffer.loadFromFile("./things/audio/usemoney.wav"))
	{
		std::cout << "花钱音乐加载失败" << std::endl;
	}
	UseMoneyMusic.setBuffer(UseMoneyMusicBuffer);
	//音频加载结束
	if (!font.loadFromFile("./things/font/simsun.ttc"))
	{
		std::cout << "字体加载失败" << std::endl;
	}
	text.setFont(font);
	//资源初始化
	if (!tLoadingPic.loadFromFile("./things/img/Sbk.png")) 
	{
		std::cout << "开始背景图加载失败" << std::endl;
	}
	spLoadingPic.setTexture(tLoadingPic);
	spLoadingPic.setScale(SCALE, SCALE);

	if (!tGameTest.loadFromFile("./things/img/GameTest.png"))
	{
		std::cout << "游戏说明图加载失败" << std::endl;
	}
	spGameTest.setTexture(tGameTest);
	spGameTest.setScale(1, 1);

	if (!tGamingBK.loadFromFile("./things/img/BK.png")) 
	{
		std::cout << "游戏中背景图加载失败" << std::endl;
	}
	spGamingBK.setTexture(tGamingBK);
	spGamingBK.setScale(SCALE, SCALE);

	if (!tBlueBackGround.loadFromFile("./things/img/Blue.png"))
	{
		std::cout << "蓝色背景图加载失败" << std::endl;
	}
	spBlueBackGround.setTexture(tBlueBackGround);
	spBlueBackGround.setScale(SCALE, SCALE);
	/*-------------特殊大蓝色背景----------------------*/
	spBigBlueBackGround.setTexture(tBlueBackGround);
	spBigBlueBackGround.setScale(2.1*SCALE, 2.1*SCALE);

	if (!tYellowBackGround.loadFromFile("./things/img/Yellow.png"))
	{
		std::cout << "黄色背景图加载失败" << std::endl;
	}
	spYellowBackGround.setTexture(tYellowBackGround);
	spYellowBackGround.setScale(SCALE, SCALE);
	/*-------------特殊大黄色背景----------------------*/
	spBigYellowBackGround.setTexture(tYellowBackGround);
	spBigYellowBackGround.setScale(2.1 * SCALE, 2.1 * SCALE);

	if (!tPlayButton_HighLight.loadFromFile("./things/img/1.png")) 
	{
		std::cout << "开始按钮1加载失败" << std::endl;
	}
	spPlayButton_Move.setTexture(tPlayButton_HighLight);
	spPlayButton_Move.setScale(0.6*SCALE, 0.6*SCALE);

	if (!tPlayButton.loadFromFile("./things/img/2.png")) 
	{
		std::cout << "开始按钮2加载失败" << std::endl;
	}
	spPlayButton.setTexture(tPlayButton);
	spPlayButton.setScale(0.6*SCALE, 0.6*SCALE);

	if (!tReStart.loadFromFile("./things/img/ReStart.png"))
	{
		std::cout << "重新开始加载失败" << std::endl;
	}
	spReStart.setTexture(tReStart);
	spReStart.setScale(0.6 * SCALE, 0.6 * SCALE);

	if (!tReStart1.loadFromFile("./things/img/ReStart1.png"))
	{
		std::cout << "重新开始暗加载失败" << std::endl;
	}
	spReStart1.setTexture(tReStart1);
	spReStart1.setScale(0.6 * SCALE, 0.6 * SCALE);

	if (!tExit.loadFromFile("./things/img/exit.png"))
	{
		std::cout << "退出按钮加载失败" << std::endl;
	}
	spExit.setTexture(tExit);
	spExit.setScale(2*SCALE, 2*SCALE);

	if (!tGouMai.loadFromFile("./things/img/GOUMAI.png"))
	{
		std::cout << "购买按钮加载失败" << std::endl;
	}
	spGouMai.setTexture(tGouMai);
	spGouMai.setScale(SCALE, SCALE);

	if (!tJuJue.loadFromFile("./things/img/GIVEUP.png"))
	{
		std::cout << "拒绝按钮加载失败" << std::endl;
	}
	spJuJue.setTexture(tJuJue);
	spJuJue.setScale(SCALE, SCALE);

	if (!tTouZi.loadFromFile("./things/img/TouZi.png"))
	{
		std::cout << "灰度骰子加载失败" << std::endl;
	}
	spTouZi.setTexture(tTouZi);
	spTouZi.setScale(SCALE, SCALE);//

	if (!tTouZiPic1.loadFromFile("./things/img/TouZi1.png"))
	{
		std::cout << "骰子点数加载失败" << std::endl;
	}
	spTouZiPic1.setTexture(tTouZiPic1);
	spTouZiPic1.setScale(1.5*SCALE, 1.5*SCALE);

	if (!tPlayer1.loadFromFile("./things/img/gou.png"))
	{
		std::cout << "玩家1头像加载失败" << std::endl;
	}
	spPlayer1.setTexture(tPlayer1);
	spPlayer1.setScale(SCALE, SCALE);

	if (!tPlayer2.loadFromFile("./things/img/gou3.png"))
	{
		std::cout << "玩家2头像加载失败" << std::endl;
	}
	spPlayer2.setTexture(tPlayer2);
	spPlayer2.setScale(SCALE, SCALE);

	if (!tPlayerMoney1.loadFromFile("./things/img/A.png"))
	{
		std::cout << "玩家1金钱加载失败" << std::endl;
	}
	spPlayerMoney1.setTexture(tPlayerMoney1);
	spPlayerMoney1.setScale(SCALE, SCALE);

	if (!tPlayerMoney2.loadFromFile("./things/img/B.png"))
	{
		std::cout << "玩家2金钱加载失败" << std::endl;
	}
	spPlayerMoney2.setTexture(tPlayerMoney2);
	spPlayerMoney2.setScale(SCALE, SCALE);

	if (!tBuding1.loadFromFile("./things/img/B1.png"))
	{
		std::cout << "建筑1加载失败" << std::endl;
	}
	spBuding1.setTexture(tBuding1);
	spBuding1.setScale(SCALE, SCALE);

	if (!tBuding2.loadFromFile("./things/img/B2.png"))
	{
		std::cout << "建筑2加载失败" << std::endl;
	}
	spBuding2.setTexture(tBuding2);
	spBuding2.setScale(SCALE, SCALE);
	if (!tBuding3.loadFromFile("./things/img/B3.png"))
	{
		std::cout << "建筑3加载失败" << std::endl;
	}
	spBuding3.setTexture(tBuding3);
	spBuding3.setScale(SCALE, SCALE);
	if (!tBuding4.loadFromFile("./things/img/B4.png"))
	{
		std::cout << "建筑4加载失败" << std::endl;
	}
	spBuding4.setTexture(tBuding4);
	spBuding4.setScale(SCALE, SCALE);
	if (!tBuding5.loadFromFile("./things/img/B5.png"))
	{
		std::cout << "建筑5加载失败" << std::endl;
	}
	spBuding5.setTexture(tBuding5);
	spBuding5.setScale(SCALE, SCALE);
	if (!tBuding6.loadFromFile("./things/img/B6.png"))
	{
		std::cout << "建筑6加载失败" << std::endl;
	}
	spBuding6.setTexture(tBuding6);
	spBuding6.setScale(SCALE, SCALE);
	if (!tBuding7.loadFromFile("./things/img/B7.png"))
	{
		std::cout << "建筑7加载失败" << std::endl;
	}
	spBuding7.setTexture(tBuding7);
	spBuding7.setScale(SCALE, SCALE);
	if (!tBuding8.loadFromFile("./things/img/B8.png"))
	{
		std::cout << "建筑8加载失败" << std::endl;
	}
	spBuding8.setTexture(tBuding8);
	spBuding8.setScale(SCALE, SCALE);
	if (!tBuding9.loadFromFile("./things/img/B9.png"))
	{
		std::cout << "建筑9加载失败" << std::endl;
	}
	spBuding9.setTexture(tBuding9);
	spBuding9.setScale(SCALE, SCALE);
	if (!tBuding10.loadFromFile("./things/img/B10.png"))
	{
		std::cout << "建筑10加载失败" << std::endl;
	}
	spBuding10.setTexture(tBuding10);
	spBuding10.setScale(SCALE, SCALE);
	if (!tBuding11.loadFromFile("./things/img/B11.png"))
	{
		std::cout << "建筑11加载失败" << std::endl;
	}
	spBuding11.setTexture(tBuding11);
	spBuding11.setScale(SCALE, SCALE);
	if (!tBuding12.loadFromFile("./things/img/B12.png"))
	{
		std::cout << "建筑12加载失败" << std::endl;
	}
	spBuding12.setTexture(tBuding12);
	spBuding12.setScale(SCALE, SCALE);
	if (!tBuding13.loadFromFile("./things/img/B13.png"))
	{
		std::cout << "建筑13加载失败" << std::endl;
	}
	spBuding13.setTexture(tBuding13);
	spBuding13.setScale(SCALE, SCALE);
	if (!tBuding14.loadFromFile("./things/img/B14.png"))
	{
		std::cout << "建筑14加载失败" << std::endl;
	}
	spBuding14.setTexture(tBuding14);
	spBuding14.setScale(SCALE, SCALE);
	if (!tBuding15.loadFromFile("./things/img/B15.png"))
	{
		std::cout << "建筑15加载失败" << std::endl;
	}
	spBuding15.setTexture(tBuding15);
	spBuding15.setScale(SCALE, SCALE);
	if (!tBuding16.loadFromFile("./things/img/B16.png"))
	{
		std::cout << "建筑16加载失败" << std::endl;
	}
	spBuding16.setTexture(tBuding16);
	spBuding16.setScale(SCALE, SCALE);
	if (!tBuding17.loadFromFile("./things/img/B17.png"))
	{
		std::cout << "建筑17加载失败" << std::endl;
	}
	spBuding17.setTexture(tBuding17);
	spBuding17.setScale(SCALE, SCALE);
	if (!tBuding18.loadFromFile("./things/img/B18.png"))
	{
		std::cout << "建筑18加载失败" << std::endl;
	}
	spBuding18.setTexture(tBuding18);
	spBuding18.setScale(SCALE, SCALE);
	if (!tBuding19.loadFromFile("./things/img/B19.png"))
	{
		std::cout << "建筑19加载失败" << std::endl;
	}
	spBuding19.setTexture(tBuding19);
	spBuding19.setScale(SCALE, SCALE);
	if (!tBuding20.loadFromFile("./things/img/B20.png"))
	{
		std::cout << "建筑20加载失败" << std::endl;
	}
	spBuding20.setTexture(tBuding20);
	spBuding20.setScale(SCALE, SCALE);
	if (!tBuding21.loadFromFile("./things/img/B21.png"))
	{
		std::cout << "建筑21加载失败" << std::endl;
	}
	spBuding21.setTexture(tBuding21);
	spBuding21.setScale(SCALE, SCALE);
	if (!tBuding22.loadFromFile("./things/img/B22.png"))
	{
		std::cout << "建筑22加载失败" << std::endl;
	}
	spBuding22.setTexture(tBuding22);
	spBuding22.setScale(SCALE, SCALE);
	//图片初始化结束

	//资源初始化结束

	//布尔变量初始化
	InitialInt();

	
}

void BuyLogic()
{
	if (Turn_Player1 != true)
	{
		if (L1 == 5 || L1 == 6 || L1 == 10 || L1 == 11 || L1 == 14 || L1 == 15 || L1 == 17 || L1 == 18 || L1 == 22 || L1 == 23 || L1 == 24 || L1 == 28 || L1 == 29 || L1 == 33 || L1 == 34 || L1 == 35 || L1 == 40 || L1 == 41 || L1 == 43 || L1 == 44 || L1 == 47 || L1 == 48)
		{
			if (L1 == 5)
			{
				if (Belong1 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong1 == 1)
					CostMoney = 0;
				else
				{
					CostMoney = 100;
					IsBuy = true;
				}
			}
			if (L1 == 6)
			{
				if (Belong2 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong2 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}
			}
			if (L1 == 10)
			{
				if (Belong4 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong4 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}

			}
			if (L1 == 11)
			{
				if (Belong5 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong5 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}

			}
			if (L1 == 14)
			{
				if (Belong6 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong6 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}

			}
			if (L1 == 15)
			{
				if (Belong7 == 0)
				{
					IfBuy = true;
					CostMoney = 1500;
				}
				else if (Belong7 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 300;
				}

			}
			if (L1 == 17)
			{
				if (Belong8 == 0)
				{
					IfBuy = true;
					CostMoney = 2000;
				}
				else if (Belong8 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 400;
				}

			}

			if (L1 == 18)
			{
				if (Belong9 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong9 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}

			}
			if (L1 == 22)
			{
				if (Belong10 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong10 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}

			}
			if (L1 == 23)
			{
				if (Belong11 == 0)
				{
					IfBuy = true;
					CostMoney = 1500;
				}
				else if (Belong11 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 300;
				}

			}
			if (L1 == 24)
			{
				if (Belong12 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong12 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}

			}
			if (L1 == 28 || L1 == 29)
			{
				if (Belong13 == 0)
				{
					IfBuy = true;
					CostMoney = 3000;
				}
				else if (Belong13 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 600;
				}

			}
			if (L1 == 33)
			{
				if (Belong14 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong14 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}
			}
			if (L1 == 34)
			{
				if (Belong15 == 0)
				{
					IfBuy = true;
					CostMoney = 2000;
				}
				else if (Belong15 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 400;
				}
			}
			if (L1 == 35)
			{
				if (Belong16 == 0)
				{
					IfBuy = true;
					CostMoney = 3000;
				}
				else if (Belong16 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 600;
				}
			}
			if (L1 == 40)
			{
				if (Belong17 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong17 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}
			}
			if (L1 == 41)
			{
				if (Belong18 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong18 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}
			}
			if (L1 == 43)
			{
				if (Belong19 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong19 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}
			}
			if (L1 == 44)
			{
				if (Belong20 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong20 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}
			}
			if (L1 == 47)
			{
				if (Belong21 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong21 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}
			}
			if (L1 == 48)
			{
				if (Belong22 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong22 == 1)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}
			}
		}
	}
	else
	{
		if (L2 == 5 || L2 == 6 || L2 == 10 || L2 == 11 || L2 == 14 || L2 == 15 || L2 == 17 || L2 == 18 || L2 == 22 || L2 == 23 || L2 == 24 || L2 == 28 || L2 == 29 || L2 == 33 || L2 == 34 || L2 == 35 || L2 == 40 || L2 == 41 || L2 == 43 || L2 == 44 || L2 == 47 || L2 == 48)
		{
			if (L2 == 5)
			{
				if (Belong1 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong1 == 2)
					CostMoney = 0;
				else
				{
					CostMoney = 100;
					IsBuy = true;
				}
			}
			if (L2 == 6)
			{
				if (Belong2 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong2 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}
			}
			if (L2 == 10)
			{
				if (Belong4 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong4 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}

			}
			if (L2 == 11)
			{
				if (Belong5 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong5 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}

			}
			if (L2 == 14)
			{
				if (Belong6 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong6 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}

			}
			if (L2 == 15)
			{
				if (Belong7 == 0)
				{
					IfBuy = true;
					CostMoney = 1500;
				}
				else if (Belong7 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 300;
				}

			}
			if (L2 == 17)
			{
				if (Belong8 == 0)
				{
					IfBuy = true;
					CostMoney = 2000;
				}
				else if (Belong8 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 400;
				}

			}

			if (L2 == 18)
			{
				if (Belong9 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong9 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}

			}
			if (L2 == 22)
			{
				if (Belong10 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong10 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}

			}
			if (L2 == 23)
			{
				if (Belong11 == 0)
				{
					IfBuy = true;
					CostMoney = 1500;
				}
				else if (Belong11 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 300;
				}

			}
			if (L2 == 24)
			{
				if (Belong12 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong12 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}

			}
			if (L2 == 28 || L2 == 29)
			{
				if (Belong13 == 0)
				{
					IfBuy = true;
					CostMoney = 3000;
				}
				else if (Belong13 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 600;
				}

			}
			if (L2 == 33)
			{
				if (Belong14 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong14 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}
			}
			if (L2 == 34)
			{
				if (Belong15 == 0)
				{
					IfBuy = true;
					CostMoney = 2000;
				}
				else if (Belong15 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 400;
				}
			}
			if (L2 == 35)
			{
				if (Belong16 == 0)
				{
					IfBuy = true;
					CostMoney = 3000;
				}
				else if (Belong16 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 600;
				}
			}
			if (L2 == 40)
			{
				if (Belong17 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong17 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}
			}
			if (L2 == 41)
			{
				if (Belong18 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong18 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}
			}
			if (L2 == 43)
			{
				if (Belong19 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong19 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}
			}
			if (L2 == 44)
			{
				if (Belong20 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong20 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}
			}
			if (L2 == 47)
			{
				if (Belong21 == 0)
				{
					IfBuy = true;
					CostMoney = 1000;
				}
				else if (Belong21 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 200;
				}
			}
			if (L2 == 48)
			{
				if (Belong22 == 0)
				{
					IfBuy = true;
					CostMoney = 500;
				}
				else if (Belong22 == 2)
					CostMoney = 0;
				else
				{
					IsBuy = true;
					CostMoney = 100;
				}
			}
		}
	}
}

void CostMoneyLogic()
{
	if (IsBuy && !HaveBuy)
	{
		if (!Turn_Player1)
		{
			MoneyPlayerOne -= CostMoney;
			if (L1 == 5 && CostMoney == 500)
				Belong1 = 1;

			if (L1 == 6 && CostMoney == 1000)
				Belong2 = 1;
			if (L1 == 10 && CostMoney == 1000)
				Belong4 = 1;
			if (L1 == 11 && CostMoney == 500)
				Belong5 = 1;
			if (L1 == 14 && CostMoney == 500)
				Belong6 = 1;
			if (L1 == 15 && CostMoney == 1500)
				Belong7 = 1;
			if (L1 == 17 && CostMoney == 2000)
				Belong8 = 1;
			if (L1 == 18 && CostMoney == 500)
				Belong9 = 1;
			if (L1 == 22 && CostMoney == 1000)
				Belong10 = 1;
			if (L1 == 23 && CostMoney == 1500)
				Belong11 = 1;
			if (L1 == 24 && CostMoney == 1000)
				Belong12 = 1;
			if ((L1 == 28 || L1 == 29) && CostMoney == 3000)
				Belong13 = 1;
			if (L1 == 33 && CostMoney == 500)
				Belong14 = 1;
			if (L1 == 34 && CostMoney == 2000)
				Belong15 = 1;
			if (L1 == 35 && CostMoney == 3000)
				Belong16 = 1;
			if (L1 == 40 && CostMoney == 1000)
				Belong17 = 1;
			if (L1 == 41 && CostMoney == 500)
				Belong18 = 1;
			if (L1 == 43 && CostMoney == 500)
				Belong19 = 1;
			if (L1 == 44 && CostMoney == 500)
				Belong20 = 1;
			if (L1 == 47 && CostMoney == 1000)
				Belong21 = 1;
			if (L1 == 48 && CostMoney == 500)
				Belong22 = 1;

		}
		else
		{
			MoneyPlayerTwo -= CostMoney;
			if (L2 == 5 && CostMoney == 500)
				Belong1 = 2;
			if (L2 == 6 && CostMoney == 1000)
				Belong2 = 2;
			if (L2 == 10 && CostMoney == 1000)
				Belong4 = 2;
			if (L2 == 11 && CostMoney == 500)
				Belong5 = 2;
			if (L2 == 14 && CostMoney == 500)
				Belong6 = 2;
			if (L2 == 15 && CostMoney == 1500)
				Belong7 = 2;
			if (L2 == 17 && CostMoney == 2000)
				Belong8 = 2;
			if (L2 == 18 && CostMoney == 500)
				Belong9 = 2;
			if (L2 == 22 && CostMoney == 1000)
				Belong10 = 2;
			if (L2 == 23 && CostMoney == 1500)
				Belong11 = 2;
			if (L2 == 24 && CostMoney == 1000)
				Belong12 = 2;
			if ((L2 == 28 || L2 == 29) && CostMoney == 3000)
				Belong13 = 2;
			if (L2 == 33 && CostMoney == 500)
				Belong14 = 2;
			if (L2 == 34 && CostMoney == 2000)
				Belong15 = 2;
			if (L2 == 35 && CostMoney == 3000)
				Belong16 = 2;
			if (L2 == 40 && CostMoney == 1000)
				Belong17 = 2;
			if (L2 == 41 && CostMoney == 500)
				Belong18 = 2;
			if (L2 == 43 && CostMoney == 500)
				Belong19 = 2;
			if (L2 == 44 && CostMoney == 500)
				Belong20 = 2;
			if (L2 == 47 && CostMoney == 1000)
				Belong21 = 2;
			if (L2 == 48 && CostMoney == 500)
				Belong22 = 2;
		}
		UseMoneyMusic.play();
		HaveBuy = true;
		IsBuy = false;
	}
}

void Logic()
{
	if (StartButton == true)
	{
		GameStart = true;//按下开始按钮游戏开始
	}

	if (TouZi_Click == true)//扔骰子逻辑判断
	{

		TouZiRollTime++;//记录骰子扔了一次
		TouZiTime = 0;
		TouZi_Num1 = TouZi_Num;
		srand(time(NULL));
		TouZi_Num = rand() % 6;
		//TouZi_Num = 1;
		if (TouZi_Num == 0)
		{
			TouZi_Num = 6;
		}

		int Copy_TouZiRollTime = TouZiRollTime;
		if (Copy_TouZiRollTime % 2 != 0)
		{
			Copy_TouZiRollTime = 1;
		}
		else
		{
			Copy_TouZiRollTime = 2;
		}
		std::cout << "当前为玩家";
		std::cout << Copy_TouZiRollTime;
		std::cout << "投掷" << std::endl;

		if (Copy_TouZiRollTime==1)
		{
			Turn_Player2 = true;
			Turn_Player1 = false;
		}
		else
		{
			Turn_Player1 = true;
			Turn_Player2 = false;
		}

		std::cout << "点数为";
		std::cout << TouZi_Num << std::endl;
		TouZiCartoon[0] = TouZi_Num1 - 1;
		TouZiCartoon[19] = TouZi_Num - 1;
		ChangeLocation = true;
		TouZi_Click = false;
		Roll_TouZi = true;
		TouZiClick_Ready = false;
	}
	if (TouZiTime == 19)
	{
		Roll_TouZi = false;
		BeginChange = true;//骰子转完之后再绘制玩家移动
	}

	if (ChangeLocation == true && BeginChange == true)
	{
		if (Turn_Player1 != true)
			L1++;
		else
			L2++;
		Copy_TouZi_Num++;
		if (L1 == 49)
			L1 = 0;
		if (L2 == 49)
			L2 = 0;
		Sleep(100);
	}
	if (Copy_TouZi_Num == TouZi_Num)//人物移动结束
	{
		Copy_TouZi_Num = 0;
		ChangeLocation = false;
		TouZiClick_Ready = true;//骰子可以被点击了
		BuyLogic();
		
	}
	CostMoneyLogic();
	if (L1 == 2 || L1 == 3 || L2 == 2 || L2 == 3)
	{
		if (L1 == 2 || L1 == 3)
		{
			if (MoneyPlayerOne < 1000 || MoneyPlayerOne == 1000)
			{
				IfNeedHelp = true;
				MoneyPlayerOne += 1000;
			}
		}
		else if (L2 == 2 || L2 == 3)
		{
			if(MoneyPlayerTwo < 1000 || MoneyPlayerTwo == 1000)
			{
				IfNeedHelp = true;
				MoneyPlayerTwo += 1000;
			}
		}
	}
	if (MoneyPlayerOne <0 || MoneyPlayerTwo < 0||MoneyPlayerOne==0||MoneyPlayerTwo==0)
	{
		GameOver = true;
		IfBuy = false;
	}
}

void DrawBuding()
{
	if (Belong1 == 1)
	{
		spBlueBackGround.setPosition(420 * SCALE, 1030 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong1 == 2)
	{
		spYellowBackGround.setPosition(420 * SCALE, 1030 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding1.setPosition(420 * SCALE, 1030 * SCALE);
	window.draw(spBuding1);
	if (Belong2 == 1)
	{
		spBlueBackGround.setPosition(305 * SCALE, 1030 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong2 == 2)
	{
		spYellowBackGround.setPosition(305 * SCALE, 1030 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding2.setPosition(320 * SCALE, 1030 * SCALE);
	window.draw(spBuding2);
	/*------------银行 还没做--------------------------*/
	spBuding3.setPosition(660 * SCALE, 940 * SCALE);
	window.draw(spBuding3);
	/*------------银行 还没做--------------------------*/
	if (Belong4 == 1)
	{
		spBlueBackGround.setPosition(195 * SCALE, 915 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong4 == 2)
	{
		spYellowBackGround.setPosition(195 * SCALE, 915 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding4.setPosition(200 * SCALE, 920 * SCALE);
	window.draw(spBuding4);
	if (Belong5 == 1)
	{
		spBlueBackGround.setPosition(190 * SCALE, 805 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong5 == 2)
	{
		spYellowBackGround.setPosition(190 * SCALE, 805 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding5.setPosition(190 * SCALE, 810 * SCALE);
	window.draw(spBuding5);
	if (Belong6 == 1)
	{
		spBlueBackGround.setPosition(190 * SCALE, 470 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong6 == 2)
	{
		spYellowBackGround.setPosition(190 * SCALE, 470 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding6.setPosition(190 * SCALE, 470 * SCALE);
	window.draw(spBuding6);
	if (Belong7 == 1)
	{
		spBlueBackGround.setPosition(305 * SCALE, 705 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong7 == 2)
	{
		spYellowBackGround.setPosition(305 * SCALE, 705 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding7.setPosition(305 * SCALE, 700 * SCALE);
	window.draw(spBuding7);
	if (Belong8 == 1)
	{
		spBlueBackGround.setPosition(540 * SCALE, 470 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong8 == 2)
	{
		spYellowBackGround.setPosition(540 * SCALE, 470 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding8.setPosition(540 * SCALE, 470 * SCALE);
	window.draw(spBuding8);
	if (Belong9 == 1)
	{
		spBlueBackGround.setPosition(540 * SCALE, 365 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong9 == 2)
	{
		spYellowBackGround.setPosition(540 * SCALE, 365 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding9.setPosition(540 * SCALE, 370 * SCALE);
	window.draw(spBuding9);
	if (Belong10 == 1)
	{
		spBlueBackGround.setPosition(310 * SCALE, 10 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong10 == 2)
	{
		spYellowBackGround.setPosition(310 * SCALE, 10 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding10.setPosition(310 * SCALE, 10 * SCALE);
	window.draw(spBuding10);
	if (Belong11 == 1)
	{
		spBlueBackGround.setPosition(420 * SCALE, 20 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong11 == 2)
	{
		spYellowBackGround.setPosition(420 * SCALE, 20 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding11.setPosition(430 * SCALE, 30 * SCALE);
	window.draw(spBuding11);
	if (Belong12 == 1)
	{
		spBlueBackGround.setPosition(540 * SCALE, 20 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong12 == 2)
	{
		spYellowBackGround.setPosition(540 * SCALE, 20 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding12.setPosition(540 * SCALE, 20 * SCALE);
	window.draw(spBuding12);
	/*----------------------大建筑的SCALE还没做----------------*/
	if (Belong13 == 1)
	{
		spBigBlueBackGround.setPosition(765 * SCALE, 140 * SCALE);
		window.draw(spBigBlueBackGround);
	}
	if (Belong13 == 2)
	{
		spBigYellowBackGround.setPosition(765 * SCALE, 140 * SCALE);
		window.draw(spBigYellowBackGround);
	}
	spBuding13.setPosition(765 * SCALE, 140 * SCALE);
	window.draw(spBuding13);
	if (Belong14 == 1)
	{
		spBlueBackGround.setPosition(1100 * SCALE, 20 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong14 == 2)
	{
		spYellowBackGround.setPosition(1100 * SCALE, 20 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding14.setPosition(1100 * SCALE, 30 * SCALE);
	window.draw(spBuding14);
	if (Belong15 == 1)
	{
		spBlueBackGround.setPosition(1220 * SCALE, 20 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong15 == 2)
	{
		spYellowBackGround.setPosition(1220 * SCALE, 20 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding15.setPosition(1220 * SCALE, 30 * SCALE);
	window.draw(spBuding15);
	if (Belong16 == 1)
	{
		spBlueBackGround.setPosition(1335 * SCALE, 25 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong16 == 2)
	{
		spYellowBackGround.setPosition(1335 * SCALE, 25 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding16.setPosition(1340 * SCALE, 30 * SCALE);
	window.draw(spBuding16);
	if (Belong17 == 1)
	{
		spBlueBackGround.setPosition(1325 * SCALE, 365 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong17 == 2)
	{
		spYellowBackGround.setPosition(1325 * SCALE, 365 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding17.setPosition(1330 * SCALE, 370 * SCALE);
	window.draw(spBuding17);
	if (Belong18 == 1)
	{
		spBlueBackGround.setPosition(1210 * SCALE, 365 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong18 == 2)
	{
		spYellowBackGround.setPosition(1210 * SCALE, 365 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding18.setPosition(1210 * SCALE, 370 * SCALE);
	window.draw(spBuding18);
	if (Belong19 == 1)
	{
		spBlueBackGround.setPosition(980 * SCALE, 590 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong19 == 2)
	{
		spYellowBackGround.setPosition(980 * SCALE, 590 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding19.setPosition(980 * SCALE, 590 * SCALE);
	window.draw(spBuding19);
	if (Belong20 == 1)
	{
		spBlueBackGround.setPosition(980 * SCALE, 700 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong20 == 2)
	{
		spYellowBackGround.setPosition(980 * SCALE, 700 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding20.setPosition(980 * SCALE, 700 * SCALE);
	window.draw(spBuding20);
	if (Belong21 == 1)
	{
		spBlueBackGround.setPosition(1105 * SCALE, 925 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong21 == 2)
	{
		spYellowBackGround.setPosition(1105 * SCALE, 925 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding21.setPosition(1105 * SCALE, 925 * SCALE);
	window.draw(spBuding21);
	if (Belong22 == 1)
	{
		spBlueBackGround.setPosition(1105 * SCALE, 1040 * SCALE);
		window.draw(spBlueBackGround);
	}
	if (Belong22 == 2)
	{
		spYellowBackGround.setPosition(1105 * SCALE, 1040 * SCALE);
		window.draw(spYellowBackGround);
	}
	spBuding22.setPosition(1105 * SCALE, 1040 * SCALE);
	window.draw(spBuding22);
}

void Draw()
{
	if (!GameStart)//游戏未开始按钮图片切换
	{
		spLoadingPic.setPosition(0, 0);
		window.draw(spLoadingPic);//开始界面背景图
		spPlayButton.setPosition(1000*SCALE, 780*SCALE);
		window.draw(spPlayButton);//开始选项
		if (StartButton_Move == true)
		{
			spPlayButton_Move.setPosition(1000 * SCALE, 780 * SCALE);//鼠标滑过图片
			window.draw(spPlayButton_Move);
		}
	}
	else
	{
		if (!GameOver||!PicGameOver)
		{
			/*-------------------字体设置-------------------------*/
			int CharSize = 40;
			text.setCharacterSize(CharSize);
			text.setFillColor(Color(255, 255, 255, 255));
			text.setStyle(Text::Bold);
			/*---------------------背景----------------------------*/
			spGamingBK.setPosition(0, 0);
			window.draw(spGamingBK);
			/*---------------------建筑绘制------------------------------*/
			DrawBuding();
			/*-----------------------玩家绘制--------------------------------*/
			int X1 = LocationX[L1], Y1 = LocationY[L1];
			spPlayer1.setPosition(X1 * SCALE, Y1 * SCALE);
			window.draw(spPlayer1);//玩家1的坐标绘制
			int X2 = LocationX[L2], Y2 = LocationY[L2];
			if (L2 == L1)
			{
				Y2 -= 10;
			}
			spPlayer2.setPosition(X2 * SCALE, Y2 * SCALE);
			window.draw(spPlayer2);//玩家2坐标绘制
			BeginChange = false;
			//移动判定结束
			if (TouZiTime == 19)
			{
				CharSize = 100;
				text.setPosition(1400 * SCALE, 600 * SCALE);
				text.setCharacterSize(CharSize);
				std::stringstream num;
				num << TouZi_Num;
				text.setString(num.str());	window.draw(text);
			}
			/*---------------------------玩家金钱-------------------------------*/
			CharSize = 40;
			spPlayerMoney1.setPosition(1600 * SCALE,400  * SCALE);
			window.draw(spPlayerMoney1);
			text.setPosition(1700 * SCALE, 440 * SCALE);
			text.setCharacterSize(CharSize);
			std::stringstream one;
			one << MoneyPlayerOne;
			text.setString(one.str());	window.draw(text);
			//玩家1金钱坐标绘制

			spPlayerMoney2.setPosition(1600 * SCALE, 600 * SCALE);
			window.draw(spPlayerMoney2);
			text.setPosition(1700 * SCALE, 640 * SCALE);
			text.setCharacterSize(CharSize);
			std::stringstream two;
			two << MoneyPlayerTwo;
			text.setString(two.str());	window.draw(text);
			//玩家1金钱坐标绘制

			if (TouZi_Move == true)
			{
				spTouZi.setPosition(1630 * SCALE, 26 * SCALE);
				window.draw(spTouZi);//鼠标滑过骰子
			}
			/*-------------------------骰子动画--------------------------------------*/
			Vector2i LeftCorner;
			int ButtonWidth = 186;
			int ButtonHeight = 186;

			LeftCorner.x = 612;
			LeftCorner.y = 395;
			Broll.left = LeftCorner.x;
			Broll.top = LeftCorner.y;
			Broll.width = ButtonWidth;
			Broll.height = ButtonHeight;

			spTouZiPic1.setTextureRect(IntRect(0,TouZiCartoon[TouZiTime] * ButtonWidth, ButtonWidth, ButtonHeight));
			if (Roll_TouZi == true&&TouZiTime>=0&&TouZiTime<19)
			{
				TouZiTime++;
			}
			spTouZiPic1.setPosition(830, 480);
			window.draw(spTouZiPic1);
			/*------------------------购买界面---------------------------------*/
			if (IfBuy)
			{
				CharSize = 40;
				text.setPosition(700 * SCALE, 550 * SCALE);
				text.setCharacterSize(CharSize);
				std::stringstream Money;
				Money << CostMoney;
				text.setString(Money.str());	window.draw(text);

				text.setPosition(600 * SCALE, 650 * SCALE);
				text.setCharacterSize(CharSize);
				text.setString(L"是否购买？");	window.draw(text);
				spGouMai.setPosition(500 * SCALE, 750 * SCALE);
				window.draw(spGouMai);
				spJuJue.setPosition(750 * SCALE, 750 * SCALE);
				window.draw(spJuJue);
			}
			/*---------------------退出按钮-------------------------------------*/
			spExit.setPosition(1750 * SCALE, 1100 * SCALE);
			window.draw(spExit);

			if (GameOver)
			{
				/*-----------------重新开始---------------------------------*/
				PicGameOver = true;
				spReStart.setPosition(550 * SCALE, 600 * SCALE);
				window.draw(spReStart);//开始选项
			}
			spGameTest.setPosition(1999 * SCALE, 0);
			window.draw(spGameTest);//开始界面背景图
		}
	}
	window.display();
}

void Input() 
{
	text.setCharacterSize(24);
	text.setFillColor(Color(0, 0, 0, 255));

	Event event;
	while (window.pollEvent(event)) 
	{
		if (event.type == Event::Closed) 
		{
			window.close();
		}
	}
	if (event.type == Event::KeyReleased) 
	{		//键盘抬起事件
		switch (event.key.code) 
		{
		case Keyboard::Escape:		// "Esc"退出游戏
			window.close();
		default:
			break;
		}
	}

	if (!GameStart)//游戏开始界面
	{
		Loadposition = Mouse::getPosition(window);		//鼠标移过事件
		if ((Loadposition.x >= 1000*SCALE && Loadposition.x <= 1000*SCALE+699 * 0.6 *SCALE) && (Loadposition.y >= 780*SCALE && Loadposition.y <= 780*SCALE+308 * 0.6 *SCALE))
		{
			StartButton_Move = true;
		}
		else 
		{
			StartButton_Move = false;
		}

		if (event.type == Event::MouseButtonReleased)//鼠标抬起事件
		{	

			if ((Loadposition.x >= 1000*SCALE && Loadposition.x <= 1000*SCALE+(699 * 0.6 *SCALE)) && (Loadposition.y >= 780*SCALE && Loadposition.y <= 780*SCALE+(308 * 0.6 *SCALE)))
			{
				StartButton = true;
				std::cout << "游戏开始"<<std::endl;
				ClickMusic.play();
			}
		}
	}

	else if (!GameOver)//游戏进行中界面
	{
		Loadposition = Mouse::getPosition(window);		//鼠标移过骰子
		if ((Loadposition.x >= 1635*SCALE && Loadposition.x <= 1635 * SCALE + (350 * SCALE)) && (Loadposition.y >= 30 * SCALE && Loadposition.y <= 30 * SCALE + (350 * SCALE)))
		{
			TouZi_Move = true;
		}
		else
		{
			TouZi_Move = false;
		}

		if (event.type == Event::MouseButtonReleased)//鼠标骰子抬起
		{
			if ((Loadposition.x >= 1635 * SCALE && Loadposition.x <= 1635 * SCALE + (350 * SCALE)) && (Loadposition.y >= 30 * SCALE && Loadposition.y <= 30 * SCALE + (350 * SCALE))&&TouZiClick_Ready==true&&!IfBuy)
			{
				TouZi_Click = true;
				std::cout << "投掷骰子" << std::endl;;
				HaveBuy = false;
				ClickMusic.play();
			}
			if (IfBuy)
			{
				if ((Loadposition.x >= 550 * SCALE && Loadposition.x <= 550 * SCALE + (206 * SCALE)) && (Loadposition.y >= 750 * SCALE && Loadposition.y <= 750 * SCALE + (80 * SCALE))&&!HaveBuy)
				{
					std::cout << "购买" << std::endl;
					IsBuy = true;
					IfBuy = false;
				}
				if ((Loadposition.x >= 750 * SCALE && Loadposition.x <= 750 * SCALE + (206 * SCALE)) && (Loadposition.y >= 750 * SCALE && Loadposition.y <= 750 * SCALE + (80 * SCALE))&&!HaveBuy)
				{
					std::cout << "不购买" << std::endl;
					CostMoney = 0;
					IsBuy = false;
					IfBuy = false;
				}
			}
		}
		//std::cout << Loadposition.x << std::endl;
		//std::cout << Loadposition.y << std::endl;
	}
	if (event.type == Event::MouseButtonReleased)//鼠标骰子抬起
	{
		if ((Loadposition.x >= 1750 * SCALE && Loadposition.x <= 1750 * SCALE + (144 * SCALE)) && (Loadposition.y >= 1100 * SCALE && Loadposition.y <= 1100 * SCALE + (144 * SCALE)))//退出按钮
		{
			GameOver = true;
			GameQuit = true;//游戏结束
		}
	}


}

void GameOverInfo()
{
	if (!GameQuit)
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		Loadposition = Mouse::getPosition(window);		//鼠标移过事件
		if (event.type == Event::MouseButtonReleased)//鼠标骰子抬起
		{
			if ((Loadposition.x >= 550*SCALE && Loadposition.x <= 550*SCALE + (699 * SCALE)) && (Loadposition.y >= 600*SCALE && Loadposition.y <= 600*SCALE + (308 * SCALE)))
			{
				initial();
				GameStart = true;
				GameOver = false;
			}
			if ((Loadposition.x >= 1750 * SCALE && Loadposition.x <= 1750 * SCALE + (144 * SCALE)) && (Loadposition.y >= 1100 * SCALE && Loadposition.y <= 1100 * SCALE + (144 * SCALE)))//退出按钮
			{
				GameOver = true;
				GameQuit = true;//游戏结束
			}
		}
	}
}

int main() 
{
	initial();
	do 
	{
		while (window.isOpen() && GameOver == false) 
		{
			Input();
			Logic();
			Draw();
			Sleep(30);
		}
		GameOverInfo();
	} while (!GameQuit);

	return 0;
}
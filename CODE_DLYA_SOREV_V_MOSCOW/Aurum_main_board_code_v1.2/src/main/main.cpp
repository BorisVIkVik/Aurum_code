#include "project_config.h"
#include <pins_setup.h>
#include <MOTOR_control.h>
#include <motion_control.h>
#include <mathematics.h>
#include <time_service.h>
#include <adc.h>
#include <dma.h>
#include <uart.h>
#include <softI2C.h>
#include <strategy.h>
#include <gyroscope.h>
#include <camera.h>
#include <lower_board.h>
#include <IRlocator.h>
#include <bluetooth_send.h>
#include <bluetooth_robot.h>

#define MOTOR1 2
#define MOTOR2 1
#define MOTOR3 3
#define MOTOR4 0

#define CHANNEL1 1
#define CHANNEL2 2
#define CHANNEL3 3
#define CHANNEL4 4
#define LEFT 0
#define RIGHT 1

#define READ 0

#define GOALKEEPER 0
#define ATTACKER 1
#define DRIBBLER 2
#define CODE ATTACKER

#define YELLOW 0
#define BLUE 1
#define FIELD YELLOW


#define GYROSCOPEKOEFF -1.0

void goalkeep(Camera &cam, int &sp1, int &sp2, int &sp3, int &sp4, mathematics& math, lowerBoard &LB)
{
	int32_t distance = 0;					
	if(cam.getYellowDistance() > 23)
	{
		distance = 20;
	}
	else if (cam.getYellowDistance() < 18)
	{
		distance = -20;
	}
	if(sin(double(cam.getYellowAngle())) >= 0.2)
	{
		math.calculateSpeed(distance-90+(cam.getYellowAngle()), 1000, sp1, sp2, sp3, sp4);
	}
	else if (sin(double(cam.getYellowAngle())) <= -0.2)
	{
		math.calculateSpeed(90-distance+(cam.getYellowAngle()), 1000, sp1, sp2, sp3, sp4);
	}
	else
	{
		//if(LB.getValue())
		//{
		if(cam.getYellowDistance() > 25 && !LB.getValue())
		{
			math.calculateSpeed((cam.getYellowAngle()), 400, sp1, sp2, sp3, sp4) ;
		}							
		else if (cam.getYellowDistance() < 20 && !LB.getValue())
		{
			math.calculateSpeed(0, 400, sp1, sp2, sp3, sp4);
		}
		else
		{
			sp1 = 0;
			sp2 = 0;
			sp3 = 0;
			sp4 = 0;
		}
	}
}
//Motor4 pins pd3 - en; pd8 - inv pc6 - in1; pc7 - in2; 

int32_t min(int first, int second)
{
	if(first > second)
	{
		return second;
	}
	else
	{
		return first;
	}
}

int32_t max(int first, int second)
{
	if(first < second)
	{
		return second;
	}
	else
	{
		return first;
	}
}

bool inEnemyGoalArea(Camera &cam, Gyroscope &gyro)
{
	if(FIELD == YELLOW)
	{
		if(abs(float(cam.getYellowGLY(gyro))) < 4)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(abs(float(cam.getBlueGLY(gyro))) < 4)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool isItAngleToStartMovementToEnemyGoal(Camera cam)
{
	if(FIELD == YELLOW)
	{
		if(cam.getYellowAngle() >= atan(3.0/4.0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(cam.getBlueAngle() >= atan(3.0/4.0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

int16_t homeGoalDistance(Camera &cam)
{
	if(FIELD == YELLOW)
	{
		return cam.getBlueDistance();
	}
	else
	{
		return cam.getYellowDistance();
	}
}

int16_t homeGoalGLY(Camera &cam, Gyroscope &gyro)
{
	if(FIELD == YELLOW)
	{
		return abs(float(cam.getBlueGLY(gyro)));
	}
	else
	{
		return abs(float(cam.getYellowGLY(gyro)));
	}
}

int16_t homeGoalAngle(Camera &cam)
{
	if(FIELD == YELLOW)
	{
		return cam.getBlueAngle();
	}
	else
	{
		return cam.getYellowAngle();
	}
}

int16_t enemyGoalAngle(Camera &cam)
{
	if(FIELD == YELLOW)
	{
		return cam.getYellowAngle();
	}
	else
	{
		return cam.getBlueAngle();
	}
}

int16_t enemyGoalGLY(Camera &cam, Gyroscope &gyro)
{
	if(FIELD == BLUE)
	{
		return abs(float(cam.getBlueGLY(gyro)));
	}
	else
	{
		return abs(float(cam.getYellowGLY(gyro)));
	}
}

int main()

{
	RCC_APB2PeriphClockCmd(RCC_APB2LPENR_SYSCFGLPEN, ENABLE);
	time_service::init();
	time_service::startTime();
  time_service::delay_ms(500);
	static Pin m1In2(GPIO_Pin_9,
									GPIOE,
									GPIO_Mode_AF,
									TIM1,
									CHANNEL1,
									RCC_APB2Periph_TIM1,
									GPIO_PinSource9,
									GPIO_AF_TIM1,
									4096,
									1,
									GPIO_OType_PP,
									GPIO_PuPd_UP);
	
		static Pin dribbler(GPIO_Pin_6,
									GPIOE,
									GPIO_Mode_AF,
									TIM9,
									CHANNEL2,
									RCC_APB2Periph_TIM9,
									GPIO_PinSource6,
									GPIO_AF_TIM9,
									4096,
									1,
									GPIO_OType_PP,
									GPIO_PuPd_NOPULL);
	dribbler.pinInit();
	dribbler.pwmInit();
	static Pin m1In1(GPIO_Pin_11,
									GPIOE,
									GPIO_Mode_AF,
									TIM1,
									CHANNEL2,
									RCC_APB2Periph_TIM1,
									GPIO_PinSource11,
									GPIO_AF_TIM1,
									4096,
									1,
									GPIO_OType_PP,
									GPIO_PuPd_UP);
	static 	Motor m1(m1In1, m1In2, MOTOR1);
  static Pin m2In1(GPIO_Pin_6,
									GPIOA,
									GPIO_Mode_AF,
									TIM3,
									CHANNEL1,
									RCC_APB1Periph_TIM3,
									GPIO_PinSource6,
									GPIO_AF_TIM3,
									4096,
									1,
									GPIO_OType_PP,
									GPIO_PuPd_NOPULL);
	//Pin m1In2(GPIO_Pin_3, GPIOA, GPIO_Mode_AF, TIM5, CHANNEL2, RCC_APB1Periph_TIM5);
	static Pin m2In2(GPIO_Pin_7,
	  							 GPIOA,
									 GPIO_Mode_AF,
									 TIM3,
									 CHANNEL2,
									 RCC_APB1Periph_TIM3,
									 GPIO_PinSource7,
									 GPIO_AF_TIM3,
									 4096,
									 1,
									 GPIO_OType_PP,
									 GPIO_PuPd_NOPULL);
	static Motor m2(m2In1, m2In2, MOTOR2);
	static Pin m3In1(GPIO_Pin_15,
									GPIOD,
									GPIO_Mode_AF,
									TIM4,
									CHANNEL4,
									RCC_APB1Periph_TIM4,
									GPIO_PinSource15,
									GPIO_AF_TIM4,
									4096,
									1,
									GPIO_OType_PP,
									GPIO_PuPd_NOPULL);
	//Pin m1In2(GPIO_Pin_3, GPIOA, GPIO_Mode_AF, TIM5, CHANNEL2, RCC_APB1Periph_TIM5);
	static Pin m3In2(GPIO_Pin_14,
									GPIOD,
									GPIO_Mode_AF,
									TIM4,
									CHANNEL3,
									RCC_APB1Periph_TIM4,
									GPIO_PinSource14,
									GPIO_AF_TIM4,
									4096,
									1,
									GPIO_OType_PP,
									GPIO_PuPd_NOPULL);
	static Motor m3(m3In1, m3In2, MOTOR3);
	static Pin m4In2(GPIO_Pin_8,
									GPIOC,
									GPIO_Mode_AF,
									TIM3,
									CHANNEL3,
									RCC_APB1Periph_TIM3,
									GPIO_PinSource8,
									GPIO_AF_TIM3,
									4096,
									1,
									GPIO_OType_PP,
									GPIO_PuPd_NOPULL);
	//Pin m1In2(GPIO_Pin_3, GPIOA, GPIO_Mode_AF, TIM5, CHANNEL2, RCC_APB1Periph_TIM5);
	static Pin m4In1(GPIO_Pin_9,
									GPIOC,
									GPIO_Mode_AF,
									TIM3,
									CHANNEL4,
									RCC_APB1Periph_TIM3,
									GPIO_PinSource9,
									GPIO_AF_TIM3,
									4096,
									1,
									GPIO_OType_PP,
									GPIO_PuPd_NOPULL);
	static Motor m4(m4In1, m4In2, MOTOR4);
	static Pin ballSignal(GPIO_Pin_0,
						 GPIOC,
					 	 GPIO_Mode_AN,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource0,
						 GPIO_AF_TIM3,
						 4096,
						 1,
						 GPIO_OType_PP,
						 GPIO_PuPd_NOPULL);
//	
	static Pin txB1(GPIO_Pin_10,
						 GPIOB,
					 	 GPIO_Mode_AF,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource10,
						 GPIO_AF_USART3,
						 4096,
						 1,
						 GPIO_OType_PP,
						 GPIO_PuPd_NOPULL);
	static Pin rxB1(GPIO_Pin_11,
						 GPIOB,
					 	 GPIO_Mode_AF,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource11,
						 GPIO_AF_USART3,
						 4096,
						 1,
						 GPIO_OType_PP,
						 GPIO_PuPd_NOPULL);
	static Pin switchPin(GPIO_Pin_5,
						 GPIOA,
					 	 GPIO_Mode_IN,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource5,
						 GPIO_AF_USART1,
						 4096,
						 1,
						 GPIO_OType_PP,
						 GPIO_PuPd_UP);
	switchPin.pinInit();
	static Pin txLB(GPIO_Pin_6,
						 GPIOB,
					 	 GPIO_Mode_AF,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource6,
						 GPIO_AF_USART1,
						 4096,
						 1,
						 GPIO_OType_PP,
						 GPIO_PuPd_NOPULL);
	static Pin rxLB(GPIO_Pin_7,
						 GPIOB,
					 	 GPIO_Mode_AF,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource7,
						 GPIO_AF_USART1,
						 4096,
						 1,
						 GPIO_OType_PP,
						 GPIO_PuPd_NOPULL);
	static Pin txCam(GPIO_Pin_2,
						 GPIOA,
					 	 GPIO_Mode_AF,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource2,
						 GPIO_AF_USART2,
						 4096,
						 1,
						 GPIO_OType_PP,
						 GPIO_PuPd_NOPULL);
	static Pin rxCam(GPIO_Pin_3,
						 GPIOA,
					 	 GPIO_Mode_AF,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource3,
						 GPIO_AF_USART2,
						 4096,
						 1,
						 GPIO_OType_PP,
						 GPIO_PuPd_NOPULL);
	static Pin txGyro(GPIO_Pin_6,
						 GPIOC,
					 	 GPIO_Mode_AF,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource6,
						 GPIO_AF_USART6,
						 4096,
						 1,
						 GPIO_OType_PP,
						 GPIO_PuPd_NOPULL);
	static Pin rxGyro(GPIO_Pin_7,
						 GPIOC,
					 	 GPIO_Mode_AF,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource7,
						 GPIO_AF_USART6,
						 4096,
						 1,
						 GPIO_OType_PP,
						 GPIO_PuPd_NOPULL);
	static Pin sclIR(GPIO_Pin_8,
						 GPIOB,
					 	 GPIO_Mode_OUT,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource7,
						 GPIO_AF_USART6,
						 4096,
						 1,
						 GPIO_OType_OD,
						 GPIO_PuPd_UP);
	sclIR.pinInit();
	sclIR.setBit();
	static Pin sdaIR(GPIO_Pin_9,
						 GPIOB,
					 	 GPIO_Mode_OUT,
						 TIM3,
						 CHANNEL2,
						 RCC_APB1Periph_TIM3,
						 GPIO_PinSource7,
						 GPIO_AF_USART6,
						 4096,
						 1,
						 GPIO_OType_OD,
						 GPIO_PuPd_UP);						 
		sdaIR.pinInit();
		sdaIR.setBit();			 
		softI2C i2cIR(sclIR, sdaIR);
		IRlocator locator360(i2cIR, 0x0E);
		volatile int16_t angleIR = 0;
		volatile int16_t powerIR = 0;
		//RCC_APB1Periph_I2C1()
	Adc adcBall(ADC1, 1, 0, ADC_Channel_10, RCC_APB2Periph_ADC1, ballSignal);
	adcBall.sendMeChannel(ADC_Channel_10);
	Dma ballDma(RCC_AHB1Periph_DMA2, adcBall);
	static ballDetection ballControl(ballDma);
	robotMotion robot(m1, m2, m3, m4);
	robot.robotInit();
	static Gyroscope gyro(txGyro, rxGyro);
	volatile int16_t angle = 0;
	volatile int16_t checkAngle = 0;
	volatile int16_t savedAngle = 0;
	volatile int32_t kek = 0;
	volatile int16_t camValue = 0;
	mathematics math(locator360);
	static Camera cam(txCam, rxCam);
	lowerBoard LB(txLB, rxLB);
	BluetoothSend bS(txB1, rxB1);
	for(int32_t gyroCount = 0; gyroCount < 1250000; gyroCount++)
	{
			gyro.setAngle(0);
	}
	dribbler.pwm(1100);
	volatile int32_t dribblingValue = 1300;
	volatile int32_t time = 0;
	volatile int32_t sendTime = 0;
	time = time_service::getCurTime();
	sendTime = time_service::getCurTime();
	volatile int8_t lbValue = 0;
	volatile int32_t goalAngle = 0;
	int robotAngle = 0;
	bool side = false;
	bool push = false;
	bool start = false;
	volatile int8_t startOp = 0;
	int saveangle = 0;
	int32_t throwtimer = 0;
	while(1)
	{
		
		LB.read();
		if(CODE == ATTACKER)
		{
			cam.readData();
			int32_t sp1 = 0;
			int32_t sp2 = 0;
			int32_t sp3 = 0;
			int32_t sp4 = 0;
			angleIR = locator360.getAngle();
			//powerIR = locator360.getDistance();
			if(!switchPin.readPin())
			{	
				if(!start)
				{	
					gyro.resetGyro();
					gyro.calcStartingAngle();
					start = true;
				}
				
				if(ballControl.isThereABall())
				{
					time = time_service::getCurTime();
				}
				else
				{
					if(time_service::getCurTime() - time >= 2000)
					{
						startOp=0;
					}
				}
				gyro.setAngle(robotAngle);					
				if(angleIR != 255)
				{		
					angleIR = ((angleIR*-5 + 360 + 180) % 360);	
					if(startOp == 0)
					{
						robotAngle = 0;	
						side = false;
						dribbler.pwm(1230);
						if(ballControl.isThereABall())
						{
							startOp = 1;
						}
					}
					else if (startOp == 1)
					{
						dribbler.pwm(1200);
						robotAngle = 180;
						int32_t check = robotAngle + gyro.getStartingAngle() - gyro.getChangedAngle();
						while (check > 180)
						{
							check = check - 2*180;
						}
						while (check < -180)
						{
							check = check + 2*180;
						}
						if(abs(float(check)) < 20 && ballControl.isThereABall())
						{
							startOp = 2;
							throwtimer = time_service::getCurTime();
							if(!side)
							{
								int32_t curGoalAngle = homeGoalAngle(cam) + gyro.getStartingAngle() - gyro.getChangedAngle();
								if(curGoalAngle < 0)
								{
									saveangle = 185 - 45;
								}
								else
								{
									saveangle = 185 + 45;
								}
								side = true;
							}
							//dribbler.pwm(1400);
							//for(int i = 0 ; i < 100000; i++);
						}
					}
					else if (startOp == 2)
					{
						dribbler.pwm(1300);
						if(LB.getValue())
						{
							throwtimer = time_service::getCurTime();
							startOp = 3;
						}
					}
					else if (startOp == 3)
					{
						dribbler.pwm(1350);
						if(time_service::getCurTime() - throwtimer > 2000 && !LB.getValue())
						{
							throwtimer = time_service::getCurTime();
							startOp = 4;
						}
					}
					else if (startOp == 4)
					{
						dribbler.pwm(1350);
						if(LB.getValue() || time_service::getCurTime() - throwtimer > 1000)
						{
							throwtimer = time_service::getCurTime();
							startOp = 5;
						}
					}
					else if (startOp == 5)
					{
						dribbler.pwm(1200);
						robotAngle = saveangle;
						int32_t check = robotAngle + gyro.getStartingAngle() - gyro.getChangedAngle();
						while (check > 180)
						{
							check = check - 2*180;
						}
						while (check < -180)
						{
							check = check + 2*180;
						}
						if(!ballControl.isThereABall() || time_service::getCurTime() - throwtimer > 2000 || abs(float(check)) < 20)
						{
							side = false;
							throwtimer = time_service::getCurTime();
							startOp = 0;
						}
					}
					else if(startOp == 20)
					{
						if(abs(float(angleIR - 270)) < 3)
						{
							startOp = 21;
						}
					}
					else if (startOp == 21)
					{
						dribbler.pwm(1200);
						robotAngle = 270;
						int32_t check = robotAngle + gyro.getStartingAngle() - gyro.getChangedAngle() + 180;
						while (check > 180)
						{
							check = check - 2*180;
						}
						while (check < -180)
						{
							check = check + 2*180;
						}
						if(abs(float(check)) < 5)
						{
							side = false;
							startOp = 22;
						}
					}
					else if(startOp == 22)
					{
						if(ballControl.isThereABall())
						{
							startOp = 23;
						}
					}
					else if (startOp == 23)
					{
						//if(inEnemyGoalArea(cam))
						//{
						//	startOp = 24;
						//}
//						robotAngle = 180;
//						int32_t check = robotAngle + gyro.getStartingAngle() - gyro.getChangedAngle();
//						while (check > 180)
//						{
//							check = check - 2*180;
//						}
//						while (check < -180)
//						{
//							check = check + 2*180;
//						}
//						if(abs(float(check)) < 5)
//						{
//							side = false;
//							dribbler.pwm(1100);
							//startOp = ;
//						}
					}
					else if (startOp == 30)
					{
						if(abs(float(angleIR - 90)) < 3)
						{
							startOp = 31;
						}
					}
					else if (startOp == 31)
					{
						dribbler.pwm(1200);
						robotAngle = 90;
						int32_t check = robotAngle + gyro.getStartingAngle() - gyro.getChangedAngle() + 180;
						while (check > 180)
						{
							check = check - 2*180;
						}
						while (check < -180)
						{
							check = check + 2*180;
						}
						if(abs(float(check)) < 5)
						{
							side = false;
							startOp = 32;
						}
					}
					else if (startOp == 32)
					{
						if(ballControl.isThereABall())
						{
							startOp = 33;
						}
					}
					else if (startOp == 33)
					{
						
					}
					
						if(startOp == 0)
						{
							
							if(!LB.getValue())
							{
								if ((angleIR >= 175 && angleIR <= 185))
								{
									math.calculateSpeed(angleIR + 180, 600, sp1, sp2, sp3, sp4);
								}
								else if((angleIR >= 90 && angleIR <= 175))
								{
									math.calculateSpeed((angleIR + 90), 750, sp1, sp2, sp3, sp4);
								}
								else if ((angleIR >= 185 && angleIR <= 270))
								{
									math.calculateSpeed((angleIR - 90), 750, sp1, sp2, sp3, sp4);
								}
								else if(angleIR >= 270 && angleIR < 360)
								{
									math.calculateSpeed((angleIR - 90), 1000, sp1, sp2, sp3, sp4);
								}
								else if (angleIR <= 90 && angleIR > 0)
								{
									math.calculateSpeed((angleIR + 90), 1000, sp1, sp2, sp3, sp4);
								}
							}
							else
							{
								if(homeGoalGLY(cam, gyro) < 20)
								{
									math.calculateSpeed(0, 1000, sp1, sp2, sp3, sp4);
								}
								else if (enemyGoalGLY(cam, gyro) < 20)
								{
									math.calculateSpeed(180, 1000, sp1, sp2, sp3, sp4);
								}
								else
								{
									int32_t curGoalAngle = homeGoalAngle(cam) + gyro.getStartingAngle() - gyro.getChangedAngle();
									if(curGoalAngle < 0)
									{
										math.calculateSpeed(270, 1000, sp1, sp2, sp3, sp4);
									}
									else
									{
										math.calculateSpeed(90, 1000, sp1, sp2, sp3, sp4);
									}
								}
							}
						}
						else if (startOp == 1)
						{
							if(!LB.getValue())
							{
								int32_t curAngle = 0;
								if(gyro.getImpact() < 0)
								{
									if(ballControl.isThereABall())
									{
										math.calculateSpeed((angleIR - curAngle + 90), 750, sp1, sp2, sp3, sp4);
									}
									else
									{
										math.calculateSpeed(((angleIR - curAngle) * 2.2), 1000, sp1, sp2, sp3, sp4);
									}
								}
								else
								{
									if(ballControl.isThereABall())
									{
										math.calculateSpeed((angleIR - curAngle - 90), 750, sp1, sp2, sp3, sp4);
									}
									else
									{
										math.calculateSpeed(((angleIR - curAngle) * 2.2), 1000, sp1, sp2, sp3, sp4);
									}
								}
							}
							else
							{
								int32_t curAngle = gyro.getStartingAngle() - gyro.getChangedAngle();
								if(homeGoalGLY(cam, gyro) < 20)
								{
									math.calculateSpeed(-curAngle, 1000, sp1, sp2, sp3, sp4);
								}
								else if (enemyGoalGLY(cam, gyro) < 20)
								{
									math.calculateSpeed(180-curAngle, 1000, sp1, sp2, sp3, sp4);
								}
								else
								{
									int32_t curGoalAngle = homeGoalAngle(cam) + gyro.getStartingAngle() - gyro.getChangedAngle();
									if(curGoalAngle < 0)
									{
										math.calculateSpeed(270-curAngle, 1000, sp1, sp2, sp3, sp4);
									}
									else
									{
										math.calculateSpeed(90-curAngle, 1000, sp1, sp2, sp3, sp4);
									}
								}
							}
						}
						else if (startOp == 2)
						{
							if(saveangle == 185 - 45)
							{
								math.calculateSpeed(270, 900, sp1, sp2, sp3, sp4);
							}
							else
							{
								math.calculateSpeed(90, 900, sp1, sp2, sp3, sp4);
							}
						}
						else if (startOp == 3)
						{
							if(saveangle == 185 - 45)
							{
								math.calculateSpeed(90, 500, sp1, sp2, sp3, sp4);
							}
							else
							{
								math.calculateSpeed(270, 500, sp1, sp2, sp3, sp4);
							}
						}
						else if (startOp == 4)
						{
							math.calculateSpeed(180, 800, sp1, sp2, sp3, sp4);
						}
						else if (startOp == 5)
						{
							if(saveangle == 185 - 45)
							{
								math.calculateSpeed(90, 500, sp1, sp2, sp3, sp4);
							}
							else
							{
								math.calculateSpeed(270, 500, sp1, sp2, sp3, sp4);
							}
						}
						else if (startOp == 20)
						{
							int32_t speed = 10 * (angleIR - 270);
							if(speed < 0)
							{
								math.calculateSpeed(0, max(-speed, 200), sp1, sp2, sp3, sp4);
							}
							else
							{
								math.calculateSpeed(180, max(speed, 200), sp1, sp2, sp3, sp4);
							}
						}
						else if (startOp == 21)
						{
							sp1 = 0;
							sp2 = 0;
							sp3 = 0;
							sp4 = 0;
						}
						else if (startOp == 22)
						{
							math.calculateSpeed(0, 500, sp1, sp2, sp3, sp4);
						}
						else if (startOp == 23)
						{
							math.calculateSpeed(270, 1500, sp1, sp2, sp3, sp4);
						}
						else if (startOp == 24)
						{
							sp1 = 0;
							sp2 = 0;
							sp3 = 0;
							sp4 = 0;
						}
						else if (startOp == 30)
						{
							int32_t speed = 10 * (angleIR - 270);
							if(speed < 0)
							{
								math.calculateSpeed(0, max(-speed, 200), sp1, sp2, sp3, sp4);
							}
							else
							{
								math.calculateSpeed(180, max(speed, 200), sp1, sp2, sp3, sp4);
							}
						}
						else if (startOp == 31)
						{
							sp1 = 0;
							sp2 = 0;
							sp3 = 0;
							sp4 = 0;
						}
						else if (startOp == 32)
						{
							math.calculateSpeed(0, 500, sp1, sp2, sp3, sp4);
						}
						else if (startOp == 33)
						{
							math.calculateSpeed(90, 1500, sp1, sp2, sp3, sp4);
						}
						else if (startOp == 34)
						{
							int curAngle = 0;
							if(gyro.getImpact() < 0)
								{
									if(ballControl.isThereABall())
									{
										math.calculateSpeed((angleIR - curAngle+ 90), 1000, sp1, sp2, sp3, sp4);
									}
									else
									{
										math.calculateSpeed(((angleIR - curAngle) * 2.2), 1000, sp1, sp2, sp3, sp4);
									}
								}
								else
								{
									if(ballControl.isThereABall())
									{
										math.calculateSpeed((angleIR - curAngle - 90), 1000, sp1, sp2, sp3, sp4);
									}
									else
									{
										math.calculateSpeed(((angleIR - curAngle) * 2.2), 1000, sp1, sp2, sp3, sp4);
									}
								}
						}
					}
	//				else
//				{
//					robotAngle = 0;
//				}
			
			int raz = robotAngle + gyro.getStartingAngle() - gyro.getChangedAngle();
			while (raz > 180)
			{
				raz = raz - 2*180;
			}
			while (raz < -180)
			{
				raz = raz + 2*180;
			}
			
			kek = 15 * gyro.getImpact() / min(abs(float(raz)) + 5, 10);
			if(startOp == 1)
			{
				kek/=8;
			}
			else if (startOp == 3)
			{
				kek *= 2;
			}
			if(kek*GYROSCOPEKOEFF>1500)
			{
				kek = 1500/GYROSCOPEKOEFF;
			}
			if(kek*GYROSCOPEKOEFF<-1500)
			{
				kek = -1500/GYROSCOPEKOEFF;
			}	
			robot.move(sp1 + kek * GYROSCOPEKOEFF, sp2 + kek * GYROSCOPEKOEFF, sp3 + kek * GYROSCOPEKOEFF, sp4 + kek * GYROSCOPEKOEFF);	
		}
		else
		{
			start = false;
			sp1 = 0;
			sp2 = 0;
			sp3 = 0;
			sp4 = 0;
			startOp = 0;
			robotAngle = 0;
			robot.move(0,0,0,0);
			dribbler.pwm(1100);
		}
	}
	else if (CODE == GOALKEEPER)
	{
			cam.readData();
			int32_t sp1 = 0;
			int32_t sp2 = 0;
			int32_t sp3 = 0;
			int32_t sp4 = 0;
			angleIR = locator360.getAngle();
			powerIR = locator360.getDistance();
			if(!switchPin.readPin())
			{	
				gyro.setAngle(robotAngle);	
				if(!start)
				{	
					gyro.resetGyro();
					gyro.calcStartingAngle();
					start = true;
					dribbler.pwm(1100);
				}
				
				if(angleIR != 255)
				{	
					angleIR = ((angleIR*-5 + 360 + 180) % 360);
					double distance = 0;					
					if((homeGoalDistance(cam)) > 23)
					{
						distance = 20;
					}
					else if ((homeGoalDistance(cam)) < 18)
					{
						distance = -20;
					}
						int32_t curGoalAngle = homeGoalAngle(cam) + gyro.getStartingAngle() - gyro.getChangedAngle();
						double a = sin(double(curGoalAngle/57.3)) - sin(double(angleIR/57.3));
						if(curGoalAngle < 0 && curGoalAngle > -150 && (angleIR >= 225 || angleIR <= 90))
						{
							sp1 = 0;
							sp2 = 0;
							sp3 = 0;
							sp4 = 0;
						}
						else if(curGoalAngle > 0 && curGoalAngle < 150 && (angleIR >= 270 || angleIR <= 45))
						{
							sp1 = 0;
							sp2 = 0;
							sp3 = 0;
							sp4 = 0;
						}
						else
						{
							if(a > 0.1)
							{

								math.calculateSpeed(distance-90+(curGoalAngle), 1000, sp1, sp2, sp3, sp4);
								push = false;
							}
							else if (a < -0.1)
							{
								math.calculateSpeed(90 - distance + (curGoalAngle), 1000, sp1, sp2, sp3, sp4);
								push = false;
							}
							else
							{
								if(!push)
								{
									if ((homeGoalDistance(cam)) > 28)
									{
										push = true;
									}
									math.calculateSpeed(angleIR + 180, 1000, sp1, sp2, sp3, sp4) ;
								}
								else
								{
									if ((homeGoalDistance(cam)) > 23)
									{
										math.calculateSpeed(angleIR, 750, sp1, sp2, sp3, sp4) ;
									}
									else if ((homeGoalDistance(cam)) < 18)
									{
										math.calculateSpeed(angleIR + 180, 750, sp1, sp2, sp3, sp4) ;
									}
								}
							}
						}
					}
					else
					{
						int32_t distance = 0;					
//						if((homeGoalDistance(cam)) > 23)
//						{
//							distance = 20;
//						}
//						else if ((homeGoalDistance(cam)) < 18)
//						{
//							distance = -20;
//						}
						int32_t curGoalAngle = homeGoalAngle(cam) + gyro.getStartingAngle() - gyro.getChangedAngle();
						if(curGoalAngle < 170 && curGoalAngle > 0)
						{
							math.calculateSpeed(distance-90+(homeGoalAngle(cam)), 750, sp1, sp2, sp3, sp4);
						}
						else if (curGoalAngle > -170 && curGoalAngle < 0)
						{
							math.calculateSpeed(90-distance+(curGoalAngle), 750, sp1, sp2, sp3, sp4);
						}
						else
						{
							if ((homeGoalDistance(cam)) > 21)
							{
								int32_t speed = ((homeGoalDistance(cam)) - 21) * 50;
								if(speed > 1275)
								{
									speed = 1275;
								}
								else if ( speed < 400)
								{
									speed = 400;
								}
								math.calculateSpeed(curGoalAngle, speed, sp1, sp2, sp3, sp4) ;
							}
							else if ((homeGoalDistance(cam)) < 18)
							{
								int32_t speed = (18 - (homeGoalDistance(cam))) * 50;
								if(speed > 1275)
								{
									speed = 1275;
								}
								else if ( speed < 200)
								{
									speed = 200;
								}
								math.calculateSpeed(curGoalAngle + 180, speed, sp1, sp2, sp3, sp4) ;
							}
//							if(LB.getValue())
//							{
//								if((homeGoalDistance(cam)) > 25 && !LB.getValue())
//								{
//									math.calculateSpeed((homeGoalAngle(cam)), 400, sp1, sp2, sp3, sp4);
//								}							
//								else if ((homeGoalDistance(cam)) < 20 && !LB.getValue())
//								{
//									math.calculateSpeed((homeGoalAngle(cam) + 180), 400, sp1, sp2, sp3, sp4);
//								}
//								else
//								{
//									sp1 = 0;
//									sp2 = 0;
//									sp3 = 0;
//									sp4 = 0;
//								}
//							}
						}
					}
				int raz = robotAngle + gyro.getStartingAngle() - gyro.getChangedAngle();
				while (raz > 180)
				{
					raz = raz - 2*180;
				}
				while (raz < -180)
				{
					raz = raz + 2*180;
				}
				if(abs(float(raz)) < 1)
				{
					//dribbler.pwm(1300);
				}
				
				kek = 30 * gyro.getImpact() / min(abs(float(raz)) + 5, 10);
				if(kek*GYROSCOPEKOEFF>1000)
				{
					kek = 1000/GYROSCOPEKOEFF;
				}
				if(kek*GYROSCOPEKOEFF<-1000)
				{
					kek = -1000/GYROSCOPEKOEFF;
				}	
				robot.move(sp1 + kek * GYROSCOPEKOEFF, sp2 + kek * GYROSCOPEKOEFF, sp3 + kek * GYROSCOPEKOEFF, sp4 + kek * GYROSCOPEKOEFF);	
			}
			else
			{
				//setTurn = false;
				start = false;
				sp1 = 0;
				sp2 = 0;
				sp3 = 0;
				sp4 = 0;
				startOp = 0;
				robotAngle = 0;
				robot.move(0,0,0,0);
				dribbler.pwm(1100);
			}
		//
			
		}			
		else
		{
			if(!switchPin.readPin())
			{
				dribbler.pwm(1300);
				robot.move(0,0,0,0);
			}
			else
			{
				dribbler.pwm(1100);
			}
		}
	}
}

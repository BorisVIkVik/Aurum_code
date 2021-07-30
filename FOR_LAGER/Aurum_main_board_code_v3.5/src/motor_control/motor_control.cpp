#include "motor_control.h"
#include "time_service.h"

#define MOTOR1 1
#define MOTOR2 2
#define MOTOR3 3
#define MOTOR4 4

#define ACCELERATION 60

Motor::Motor(Pin &motorPinIn1,
             Pin &motorPinIn2,
             uint32_t motor):
             m_motorPinIn1(motorPinIn1),
             m_motorPinIn2(motorPinIn2)
{
	m_motor = motor;
}
void Motor::motorInit()
{
		curspeed = 0;
		m_accel = 0;
    m_motorPinIn1.pinInit();
		m_motorPinIn2.pinInit();
		m_motorPinIn1.pwmInit();
		m_motorPinIn2.pwmInit();
		time_ = 0;
		
}
void Motor::go(int32_t speed)
{	
		 volatile int loas = time_service::getCurTime() - time_;
		 volatile int32_t s = speed;
		 volatile int32_t cs = curspeed;
		 if(speed < 20 && speed > -20)
		 {
			 m_motorPinIn1.pwm(1000);		
			 m_motorPinIn2.pwm(1000);
			 curspeed = 0;
		 }
		 else if(speed > curspeed){
			if (curspeed < 0)
			{
					curspeed+=ACCELERATION*(time_service::getCurTime() - time_);
					int32_t cur;
					if (speed < curspeed)curspeed = speed;
					if (curspeed >0) cur = curspeed;
					else { cur = -curspeed;}
					m_motorPinIn1.pwm(cur);
					m_motorPinIn2.pwm(0);		
			
			}	
			
			else if (curspeed > 0)
			{
					curspeed+=ACCELERATION*(time_service::getCurTime() - time_);
					if (curspeed >4096)curspeed = 4096;
					if (speed < curspeed)curspeed = speed;
					m_motorPinIn1.pwm(0);
					m_motorPinIn2.pwm(curspeed);
			}
			if (curspeed == 0)curspeed++;
		}
		else if(speed < curspeed ){
			if (curspeed < 0)
			{
					curspeed-=ACCELERATION*(time_service::getCurTime() - time_);
					if (curspeed <-4096)curspeed = -4096;
					if (speed > curspeed)curspeed = speed;
					int32_t cur = -curspeed;

				m_motorPinIn1.pwm(cur);
				m_motorPinIn2.pwm(0);		
			}	
			else if (curspeed > 0)
			{
					curspeed-=ACCELERATION*(time_service::getCurTime() - time_);
					int32_t cur;
					if (speed > curspeed)curspeed = speed;
					if (curspeed <0) cur = -curspeed;
					else cur = curspeed;
					m_motorPinIn1.pwm(0);
					m_motorPinIn2.pwm(cur);
			}
			if (curspeed == 0)curspeed--;
		}
		time_ = time_service::getCurTime();	
	}


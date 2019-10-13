
#include "servo.h"

/********************************************
*@brief		Holder Init
*@param		None
*@retval	None
*********************************************/

uint16_t current_servo_x_angle; 
uint16_t current_servo_y_angle; 

/***********************************************************
*@brief		servo init 
*@parma		None
*@retval 	None
************************************************************/
void Servo_Init(void)
{
	TIM3_Init(2000-1,720-1);
}



/***********************************************************
*@brief		�� ��ʱ���Ƚ�ֵ ת��Ϊ �Ƕ� 
*@parma		val����ʱ���Ƚ�ֵ
*@retval 	���ؽǶ�
************************************************************/
double value_to_angle(uint16_t val)
{
	if(val <= X_ANGLE_180_VAL && val >= X_ANGLE_0_VAL)	// �����Ƿ���ȷ
		return (val - X_ANGLE_0_VAL) * X_ANGLE_PER_VALUE;
	else
	{
		DEBUG("Error: The value should be within %d to %d!",X_ANGLE_0_VAL,X_ANGLE_180_VAL);
		return 0;
	}
}

/***********************************************************
*@brief		�� �Ƕ� ת��Ϊ ��ʱ���Ƚ�ֵ 
*@parma		angle: �Ƕ�
*@retval 	���ض�ʱ���Ƚ�ֵ
************************************************************/
uint16_t angle_to_value(double angle)
{
	uint16_t val = 0;
		
	if(angle <= 180 && angle >= 0)
	{
		val = angle * X_VALUE_PER_ANGLE +50;
		return  val;
	}
	else
	{
		printf("Error: Angle must between 0 and 180 ! \r\n");
		return 0;
	}
	
}
/***********************************************************
*@brief		�趨 servo X ����ǰ�ĽǶ�
*@parma		angle�� �趨�ĽǶ�
*@retval 	None
************************************************************/
void Servo_Control_X(double angle)
{
	if(angle > X_MAX_ANGLE || angle < X_MIN_ANGLE)
	{
		DEBUG("The ange must between %d and %d! \r\n",X_MIN_ANGLE,X_MAX_ANGLE);
		if(angle > X_MAX_ANGLE)
			angle=X_MAX_ANGLE;
		if(angle < X_MIN_ANGLE)
			angle=X_MIN_ANGLE;
	}
	current_servo_x_angle = angle;
	TIM_SetCompare1(TIM3, angle_to_value(angle));
}


/***********************************************************
*@brief		�趨 servo Y ����ǰ�ĽǶ�
*@parma		angle�� �趨�ĽǶ�
*@retval 	None
************************************************************/
void Servo_Control_Y(double angle)
{
	if(angle > Y_MAX_ANGLE || angle < Y_MIN_ANGLE)
	{
		DEBUG("The ange must between %d and %d! \r\n",Y_MIN_ANGLE,Y_MAX_ANGLE);
		if(angle > Y_MAX_ANGLE)
			angle = Y_MAX_ANGLE;
		if(angle < Y_MIN_ANGLE)
			angle = Y_MIN_ANGLE;
	}
	current_servo_y_angle=angle;
	TIM_SetCompare2(TIM3, angle_to_value(angle));
}


/*********************************************
*@brief		��ȡ��ǰ��� X ����ĽǶ�
*@param		None
*@retval	�Ƕ�
**********************************************/
double Servo_Get_X_Angle(void)
{
	uint16_t value = 0;
	double angle = 0;
	value = TIM_GetCapture1(TIM3);	// ��ȡ�Ƚ���ֵ
	angle = value_to_angle(value);	// ���Ƚ���ת���ɽǶ�
	if(angle <= X_MIN_ANGLE)
		angle = X_MIN_ANGLE;
	else if(angle >= X_MAX_ANGLE)
		angle = X_MAX_ANGLE;
	return angle;
}


/*********************************************
*@brief		��ȡ��ǰ��� Y ����ĽǶ�
*@param		None
*@retval	�Ƕ�
**********************************************/
double Servo_Get_Y_Angle(void)
{
	uint16_t value = 0;
	double angle = 0;
	value = TIM_GetCapture2(TIM3);	// ��ȡ�Ƚ���ֵ
	angle = value_to_angle(value);  // ���Ƚ���ת���ɽǶ�
	if(angle <= Y_MIN_ANGLE)
		angle = Y_MIN_ANGLE;
	else if(angle >= Y_MAX_ANGLE)
		angle = Y_MAX_ANGLE;
	return angle;
}












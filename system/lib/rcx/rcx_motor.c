
#include<rcx_motor.h>


/*
    Name:       setMotor
    Input:   short code, which motor to affect
                short dir, what do do with the motor
    Output:     -
    Calls:  -
    Descr:      sets the staus to the motor, runing, float, stop...
*/
void setMotor(short code, short dir)
{
    char *motor= (char*)MOTOR_CONTROL;

    if(code==MOTOR_A)
    {
        if(MOTOR_FORWARD == dir)
            *motor = (*motor & 0x3f) ^ 0x80;
        if(MOTOR_BACKWARD == dir)
            *motor = (*motor & 0x3f) ^ 0x40;
        if(MOTOR_STOP == dir)
            *motor = (*motor & 0x3f) ^ 0xc0;
        if(MOTOR_FLOAT == dir)
            *motor = (*motor & 0x3f) ^ 0x00;
    }

    if(code==MOTOR_B)
    {
        switch(dir)
        {
            case MOTOR_FORWARD:
                *motor &=  ~0x04;
                *motor |=   0x08;
                break;
            case MOTOR_BACKWARD:
                *motor &=  ~0x08;
                *motor |=  0x04;
                break;
            case MOTOR_STOP:
                *motor &=  ~0x00;
                *motor |=  0x0c;
                break;
            case MOTOR_FLOAT:
                *motor &=  ~0x0c;
                *motor |=  0x00;
                break;
        }
    }

    if(code==MOTOR_C)
    {
        switch(dir)
        {
            case MOTOR_FORWARD:
                *motor &=  ~0x01;
                *motor |=   0x02;
                break;
            case MOTOR_BACKWARD:
                *motor &=  ~0x02;
                *motor |=  0x01;
                break;
            case MOTOR_STOP:
                *motor &=  ~0x00;
                *motor |=  0x03;
                break;

            case MOTOR_FLOAT:
                *motor &=  ~0x03;
                *motor |=  0x00;
                break;
        }
    }
}

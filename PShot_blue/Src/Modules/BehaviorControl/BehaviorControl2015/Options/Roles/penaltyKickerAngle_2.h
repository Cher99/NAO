#define MIN_DIS (300.0f)

#define WIDTH_EXPAND (1.2f)

#define ANGLE_50cm (1300)

float angle_turn=0.0;

//******ball's info*******
float ball_x=0.0f;//0.0
float ball_y=0.f;//0.0


int ball_status=0;
/*
0:middle
1:left
-1 right
*/

int ball_before = 0;
/*
1:left-middle
0:middle-middle
-1:right-middle
*/
float time_up = 1.0f;

//walk left or right 15cm
int alignBehind_middleBall_time =2000;//set

//angle to turn after middle align , to make nao towards ball.
int adjust_turn_middle_left = 1500;//set
int adjust_turn_middle_right = 1500;//set

//angle to adjust after walk forward about 20cm;
int angle_after_50cm = 1300;
float angle_2 = 0.52;



option(penaltyKickerAngle_2) 
{
    initial_state(lauch)
    {
        transition
        {
            if(theBallModel.estimate.position.y()>180 && state_time > 500)//ball on the left
            {
                ball_status=1;
                PlaySound("lowBattery.wav");
                goto alignBehindBall;
            }
            else if(theBallModel.estimate.position.y()< -180 && state_time > 500)//ball on the right
            {
                ball_status=-1;
                PlaySound("theMirrorCow.wav");
                goto alignBehindBall;
            }
            else if(state_time > 500)//the most happened occasion.
            {
                if(theBallModel.estimate.position.y() < -50)
                {
                    ball_before = -1;
                }
                else if(theBallModel.estimate.position.y() > 50)
                {
                    ball_before = 1;
                }
                else
                {
                    ball_before = 0;
                }
                PlaySound("penaltyKeeper.wav");
                goto alignBehindBall;
            }
        }
        action{
            theHeadControlMode = HeadControl::searchForBall;
            Stand();
        }
    }

    state(alignBehindBall)
    {
        transition
        {
            //5cm ? have great influence on which leg to kick after walking 50cm.
            if(theBallModel.estimate.position.y()<50 && theBallModel.estimate.position.y()>-50)
                goto walkToball;
        }
        action
        {
            //if angle turned too big or small,change angle speed.
            WalkToTarget(Pose2f(-0.5f, 0.05f, 0.5f), Pose2f(1.f, 0.0f, theBallModel.estimate.position.y()));
        }
    }


    state(walkToball) {
        transition{
            if (theBallModel.estimate.position.norm() < MIN_DIS) 
            {
                goto judge_angleAdjust;
            }
        }
        action{
            WalkToTarget(Pose2f(0.0f, 0.4f, 0.0f), Pose2f(0.0f, theBallModel.estimate.position.x(), 0.0f));//if cannot walk straight,change angle

        }
    }

    state(judge_angleAdjust)
    {
        transition
        {
            if(state_time >1000)
            {


                if(ball_status == 1)
                {
                    angle_after_50cm = ANGLE_50cm;//1300
                }
                //=======================================
                //=======================================
                else if(ball_status == 0)
                {
                    if(ball_before ==1)
                    {
                        angle_after_50cm = ANGLE_50cm+2000;
                    }
                    else if(ball_before== 0)
                    {
                        angle_after_50cm = ANGLE_50cm + 2000;
                    }
                    else if(ball_before == -1)
                    {
                        angle_after_50cm = ANGLE_50cm+2000;
                    }
                }
                //========================================
                //=========================================
                else if(ball_status == -1)
                {
                    angle_after_50cm = ANGLE_50cm;

                }
                goto adjust_angle;
            }
        }

        action
        {
            Stand();
        }
    }
    state(adjust_angle)
    {
        transition
        {
            if(state_time > 0)
                goto judge;
        }
        action{
            PlaySound("lowBattery.wav");
            WalkToTarget(Pose2f(0.3f, 0.0f, 0.0f), Pose2f(-1.0f, 0, 0.0f));
        }
    }

    state(judge) //adjust self position to keep ball.y,obstacle.y around +-100.f(which avoids kick out the door)
    {

        transition
        {
            if (state_time > 1000)
            {
                if(ball_status == -1)
                {
                    goto judge_l_or_r;
                }
                else if(ball_status == 0)
                {
                    if(ball_before == 1)//left
                    {
                        time_up = 0.8;
                        angle_turn = -10;
                        goto turnRight;
                    }
                    else if(ball_before == -1)//right
                    {
                        time_up=1.1;
                        angle_turn = -10;
                        goto turnRight;
                    }
                    else//middle
                    {
                        time_up=1.3;
                        angle_turn = -10;
                        goto turnRight;
                    }
                }

                else
                {
                    time_up=1.0f;
                    goto judge_l_or_r;
                }
            }

        }//transition

        action
        {
            Stand();
        }
    }

    state(turnRight) {//move to the position after play allright sound
      

        transition{
            if (theBallModel.estimate.position.norm() > 100) {
                if(state_time > 2500*time_up)
                goto judge_l_or_r;
            }
            else
                goto judge_l_or_r;
        }
        action{
            PlaySound("jump.wav");
            //???
            WalkToTarget(Pose2f(0.5f, 0.3f, 0.2f), Pose2f(angle_turn, MIN_DIS*(1.0f - float(cos(angle_2))), MIN_DIS * float(sin(angle_2))));//improve
            
        }
    }

    state(turnLeft) {
       
        transition{
            if (theBallModel.estimate.position.norm() > 100) {
                if(state_time > 2500*time_up)
                goto judge_l_or_r;
            }
            else
                goto judge_l_or_r;
        }
        action{
            PlaySound("allright.wav");
            WalkToTarget(Pose2f(0.5f, 0.3f, 0.2f), Pose2f(angle_turn, MIN_DIS* (1.0f - float(cos(angle_2))), -MIN_DIS* float(sin(angle_2))));
            
        }
    }

    state(judge_l_or_r){
        transition{
            ball_y = theBallModel.estimate.position.y();
            if (ball_y > 0){
                goto kickleft;
            }
            else {
                goto kickright;
            }


        }
        action{
            Stand();
            theHeadControlMode = HeadControl::searchObs;
        }
    }

    state(kickleft){
        transition{
            if(action_done)
                goto finish;
        }
        action{
            
            KickLeft();
        }
    }

    state(kickright){
        transition{
            if (action_done){
                goto finish;
            }
        }
        action{
            KickRight();
        }
    }

    target_state(finish) 
    {
        transition
        {
           ;
        }
        action{
            theHeadControlMode = HeadControl::searchForBall;
            Stand();
        }
    }

}

    




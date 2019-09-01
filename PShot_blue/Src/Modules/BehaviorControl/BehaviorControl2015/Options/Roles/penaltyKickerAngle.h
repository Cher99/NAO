/*
Notes:
	#time_up# is main param to adjust.It controls time to turn a big_angle or small_angle
	#angle# seems useless . #angle_turn# controls turn left or turn right.
	if there is no obstacles, robot will go back , if backtimes is 4 ,kick straight.

	Muti-Calculation has been deleted in this version.
Chen Yijiang.19.9.1
*/




#define MIN_DIS (300.0f)

//****obstacle's info*****
float center_y = 0.1f;//0.0
float left_y = 0.2f;//0.0
float right_y = 0.3f;//0.0
float width=0.0f;

//******turn angle********
float angle_turn=0.0;
float angle = 0.0f;
float small_angle = 20;

//******ball's info*******
float ball_x=0.0f;//0.0
float ball_y=0.f;//0.0

//-----------help--------
float helpx = 0.0;
float helpy = 0.0;
bool goback = false;
int backtimes = 0;
bool enough = false;
bool reach = false;
float cy=0.f,ly=0.f,ry=0.f;

int ball_status=0;
/*
0:middle
1:left
-1 right
*/

int Op_keeper = 0;
/*
0:nao-type
1:DarW-type
2:selfMade-type
*/
int ball_before = 0;

float time_up = 1.0f;


option(penaltyKickerAngle) {
    initial_state(help_control_0){
        transition{
            if(theBallModel.estimate.position.y()>150 && state_time > 500)//ball on the left
            {
                ball_status=1;
                //PlaySound("lowBattery.wav");
                goto help_control_1;
            }
            else if(theBallModel.estimate.position.y()< -150 && state_time > 500)//ball on the right
            {
                ball_status=-1;
                //PlaySound("theMirrorCow.wav");
                goto help_control_2;
            }
            else if(state_time > 500)
            {
                ball_status=0;
                if(theBallModel.estimate.position.y() < -50)
                    ball_before = -1;
                else if(theBallModel.estimate.position.y() > 50)
                    ball_before = 1;
                else
                {
                    ball_before = 0;
                }
                //PlaySound("penaltyKeeper.wav");
                goto walkToball;
            }
        }
        action{
            theHeadControlMode = HeadControl::searchForBall;
            Stand();
        }
    }

    state(help_control_1){//ball on the left
        transition{
            if(theBallModel.estimate.position.y()<7 && theBallModel.estimate.position.y()>-7)
                goto walkToball;
        }
        action{
            WalkToTarget(Pose2f(0.0f, 0.0f, 0.5f), Pose2f(0.f, 0.0f, theBallModel.estimate.position.y()));
        }
    }

    state(help_control_2){//ball on the right
        transition{
            if(theBallModel.estimate.position.y()<7 && theBallModel.estimate.position.y()>-7)
                goto walkToball;
        }
        action{
            WalkToTarget(Pose2f(0.0f, 0.0f, 0.5f), Pose2f(0.f, 0.0f, theBallModel.estimate.position.y()));
        }
    }

    state(walkToball) {
        transition{
            if (theBallModel.estimate.position.norm() < MIN_DIS) {
                if (goback == true)reach=true;
                if (enough==true){
						reach=true;
						goto judge;
            	}
            	if(goback == false)
            	{
            		goto judge;
            	}
            }
        }
        action{
            WalkToTarget(Pose2f(0.0f, 0.4f, 0.0f), Pose2f(0.0f, theBallModel.estimate.position.x(), 0.0f));

        }
    }

    state(judge) //adjust self position to keep ball.y,obstacle.y around +-100.f(which avoids kick out the door)
    {

        transition
        {
            if (state_time > 1000)
            {
                int count = theObstacleModel.obstacles.size();
                if (count != 0)
                {
                    //======================main part=============================

                    if((goback == false )||
                    	(goback == true && backtimes == 4))
                    {
                    	    // find the middle obstacle as
                            int i=1,j=0;
                            center_y=theObstacleModel.obstacles[j].center.y();
                            while(i != count){
                                cy=theObstacleModel.obstacles[i].center.y();
                                if(std::fabs(cy)<std::fabs(center_y)){
                                    j=i;
                                    center_y=theObstacleModel.obstacles[i].center.y();
                                }
                                i++;
                            }
                            center_y = theObstacleModel.obstacles[j].center.y();
                            left_y = theObstacleModel.obstacles[j].left.y();
                            right_y = theObstacleModel.obstacles[j].right.y();
                            width = left_y-right_y;


                        if(Op_keeper == 0)
                            width=width*1.2;
                         else if(Op_keeper == 1)
                            width = width*1.3;

                        if (left_y < 0 - (1.5*width)) 
                        {
                            
                                //PlaySound("notPenalized.wav");

                                if(ball_status == -1)
                                {
                                    time_up = 1.0f;
                                    goto judge_l_or_r;
                                }
                                else if(ball_status == 0)
                                {
                                    time_up = 0.8f;
                                    goto judge_l_or_r;
                                }
                                else
                                {
                                    time_up = 1.0f;
                                    goto judge_l_or_r;
                                }

                            } 
                        else if (right_y > (1.5*width)) {
                            
                                //PlaySound("penalized.wav");

                                if(ball_status == -1)
                                {
                                    time_up =0.4f;
                                    angle=small_angle*0.3;
                                    goto judge_l_or_r;
                                }
                                else if(ball_status == 0)
                                {
                                    if(ball_before == 1)
                                    {
                                        time_up = 0.5;
                                        angle_turn = 10;
                                        goto turnLeft;
                                    }
                                    else if(ball_before == -1)
                                    {
                                        time_up=0.5;
                                        angle_turn = -10;
                                        goto turnRight;
                                    }
                                    else
                                    {
                                        time_up=0.8;
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
                        else if (left_y>-(width/2.0)&& left_y <(width/2.0)) {
                            
                                angle = small_angle;
                                //PlaySound("doh.wav");
                                if(ball_status == 1)
                                {
                                    goto judge_l_or_r;
                                }
                                else if(ball_status == -1) 
                                {
                                    angle_turn=-1;
                                    time_up = 0.1;
                                    goto turnRight;
                                }
                                else if(ball_status == 0)
                                {
                                    if(ball_before == 1)//left
                                    {
                                        time_up = 0.5;
                                        angle_turn = 10;
                                        goto turnLeft;
                                    }
                                    else if(ball_before == -1)
                                    {
                                        time_up=0.5;
                                        angle_turn = -10;
                                        goto turnRight;
                                    }
                                    else
                                    {
                                        time_up=0.8;
                                        angle_turn = -10;
                                        goto turnRight;
                                    }
                                }

                            } 
                        else if (right_y > -(width/2.0) && right_y < (width/2.0)) 
                        {
                            angle = small_angle;
                            //PlaySound("high.wav");    
                            if(ball_status == 1)
                            {
                                time_up = 1.0f;
                                angle_turn=-10.f;
                                //goto turnRight;
                                goto judge_l_or_r;
                            }
                            else if(ball_status == -1)
                            {
                                time_up = 0.8;
                                angle_turn=10.f;
                                //goto turnLeft;
                                goto judge_l_or_r;
                            }
                                    
                            else if(ball_status == 0)
                            {
                                if(ball_before == 1)
                                    {
                                        time_up = 0.5;
                                        angle_turn = 1;
                                        goto turnLeft;
                                    }
                                    else if(ball_before == -1)
                                    {
                                        time_up=0.5;
                                        angle_turn = -10;
                                        goto turnRight;
                                    }
                                    else
                                    {
                                        time_up=0.8;
                                        angle_turn = -10;
                                        goto turnRight;
                                    }
                            }  

                        } 
                        else if (left_y < 0) 
                        {
                            //PlaySound("helpMe.wav");
                            if(ball_status == -1)
                            {
                                time_up =0.1f;
                                angle_turn =20;
                                goto turnLeft;
                            }
                            else if(ball_status == 0)
                            {
                                if(ball_before == 1)
                                    {
                                        time_up = 0.5;
                                        angle_turn = 10;
                                        goto turnLeft;
                                    }
                                    else if(ball_before == -1)
                                    {
                                        time_up=0.5;
                                        angle_turn = -10;
                                        goto turnRight;
                                    }
                                    else
                                    {
                                        time_up=0.8;
                                        angle_turn = -10;
                                        goto turnRight;
                                    }
                            }
                            else if(ball_status == 1)
                            {
                                time_up = 0.1f;
                                angle_turn=-10;
                                goto turnLeft;

                            }

                        } 
                        else 
                        {
                            //PlaySound("lost.wav");
                            if(ball_status == -1)
                            {
                                time_up = 0.2;
                                angle_turn = -5;
                                goto turnRight;
                            }
                            else if(ball_status == 0)
                            {
                                if(ball_before == 1)//left
                                    {
                                        time_up = 0.5;
                                        angle_turn = 10;
                                        goto turnLeft;
                                    }
                                    else if(ball_before == -1)
                                    {
                                        time_up=0.5;
                                        angle_turn = -10;
                                        goto turnRight;
                                    }
                                    else
                                    {
                                        time_up=0.8;
                                        angle_turn = -10;
                                        goto turnRight;
                                    }
                            }
                            else
                            {
                                goto judge_l_or_r;
                            }
                        }
                    }                                     
                }
                else
                {
                	if (backtimes < 4)
                    {
                            helpx=helpx + 10;
                            helpy=helpy + 10;
                            goto Goback2judge; 
                        }
                    else
                    {
                        if (enough == false){
                            enough= true;
                            goto walkToball;
                        }
                        else
                        {

                            if(ball_status == 0)
                            {
                                if(ball_before == 1)//left
                                    {
                                        time_up = 0.5;
                                        angle_turn = 10;
                                        goto turnLeft;
                                    }
                                    else if(ball_before == -1)
                                    {
                                        time_up=0.5;
                                        angle_turn = -10;
                                        goto turnRight;
                                    }
                                    else
                                    {
                                        time_up=0.8;
                                        angle_turn = -10;
                                        goto turnRight;
                                    }
                            }
                            else
                                goto judge_l_or_r;
                		}

                	}
            	}
            }
        }

        action{
                theHeadControlMode = HeadControl::searchObs;
                Stand();
            }
    }

    state(turnRight) {
      

        transition{
            if (theBallModel.estimate.position.norm() > 200) {
                if(state_time > 2500*time_up)
                goto judge_l_or_r;
            }
            else
                goto judge_l_or_r;
        }
        action{
            //PlaySound("jump.wav");
            WalkToTarget(Pose2f(-0.5f, 0.05f, 0.05f), Pose2f(angle_turn, MIN_DIS*(1.0f - float(cos(angle))), -MIN_DIS * float(sin(angle)+10)));
            
        }
    }

    state(turnLeft) {
       

        transition{
            if (theBallModel.estimate.position.norm() > 200) {
                if(state_time > 2500*time_up)
                goto judge_l_or_r;
            }
            else
                goto judge_l_or_r;
        }
        action{
            //PlaySound("allright.wav");
            WalkToTarget(Pose2f(0.5f, 0.05f, 0.05f), Pose2f(angle_turn, MIN_DIS* (1.0f - float(cos(angle))), MIN_DIS* float(sin(angle)-10)));
            
        }
    }
    state(kickForward) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            KickForward();
        }
    }

    state(kickForward_left) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            KickForward_left();
        }
    }

    state(finish) {
        transition{
            if (state_time > 3000){
                enough=false;
                goback=false;
                backtimes=0;
            }
        }
        action{
            theHeadControlMode = HeadControl::searchForBall;
            Stand();
        }
    }

    state(judge_l_or_r){
        transition{
            ball_x = theBallModel.estimate.position.x();
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

    state(Goback2judge){
        transition{
            if(state_time > 2000){
                backtimes = backtimes+1;
                goback=true;
                goto judge;
            }
        }
        action{
            WalkToTarget(Pose2f(0.f,0.5f,0.f),Pose2f(0.f,-100.f-helpx,0.f));
        }
    }

}

    




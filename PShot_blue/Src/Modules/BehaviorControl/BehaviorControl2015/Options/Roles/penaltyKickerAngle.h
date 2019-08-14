// #define MIN_DIS (300.0f)

// #define WIDTH_EXPAND (1.2f)

// //******judge info*******
// float THR_BIG = 200.0f;//xyz:200
// float THR_LIT = 100.0f;

// //****obstacle's info*****
// float center_y = 0.1f;//0.0
// float left_y = 0.2f;//0.0
// float right_y = 0.3f;//0.0
// float width=0.0f;

// float middle_y=0.0f;
// float angle = 0.0f;

// //******turn angle********
// float small_angle = 0.07f;//0.1f
// float angle_turn=0.0;

// //******ball's info*******
// float ball_x=0.0f;//0.0
// float ball_y=0.f;//0.0

// char sc[10],sl[10],sr[10],sw[10];
// char Bx[10],By[10];
// char ba[10],en[10],re[10],go[10],co[10];
// //-----------help--------
// float helpx = 0.0;
// float helpy = 0.0;
// bool goback = false;
// int backtimes = 0;
// bool enough = false;
// bool reach = false;
// float cy=0.f,ly=0.f,ry=0.f;

// int ball_status=0;
// /*
// 0:middle
// 1:left
// -1 right
// */

// int Op_keeper = 0;
// /*
// 0:nao-type
// 1:DarW-type
// 2:selfMade-type
// */
// int ball_before = 0;


// float time_up = 1.0f;


option(penaltyKickerAngle) {
//     initial_state(help_control_0){
//         transition{
//             if(theBallModel.estimate.position.y()>150 && state_time > 500)//ball on the left
//             {
//                 ball_status=1;
//                 PlaySound("lowBattery.wav");
//                 goto help_control_1;
//             }
//             else if(theBallModel.estimate.position.y()< -150 && state_time > 500)//ball on the right
//             {
//                 ball_status=-1;
//                 PlaySound("theMirrorCow.wav");
//                 goto help_control_2;
//             }
//             else if(state_time > 500)
//             {
//                 ball_status=0;
//                 if(theBallModel.estimate.position.y() < -50)
//                     ball_before = -1;
//                 else if(theBallModel.estimate.position.y() > 50)
//                     ball_before = 1;
//                 else
//                 {
//                     ball_before = 0;
//                 }
//                 PlaySound("penaltyKeeper.wav");
//                 goto walkToball;
//             }
//         }
//         action{
//             center_y = 0;
//             left_y = 0;
//             right_y = 0;
//             theHeadControlMode = HeadControl::searchForBall;
//             Stand();;
//         }
//     }

//     state(help_control_1){//ball on the left
//         transition{
//             if(theBallModel.estimate.position.y()<7 && theBallModel.estimate.position.y()>-7)
//                 goto walkToball;
//         }
//         action{
//             WalkToTarget(Pose2f(0.05f, 0.05f, 0.5f), Pose2f(theBallModel.estimate.position.angle(), 0.0f, theBallModel.estimate.position.y()));
//         }
//     }

//     state(help_control_2){//ball on the right
//         transition{
//             if(theBallModel.estimate.position.y()<7 && theBallModel.estimate.position.y()>-7)
//                 goto walkToball;
//         }
//         action{
//             WalkToTarget(Pose2f(0.05f, 0.05f, 0.5f), Pose2f(theBallModel.estimate.position.angle(), 0.0f, theBallModel.estimate.position.y()));
//         }
//     }

//     state(walkToball) {
//         transition{
//             if (theBallModel.estimate.position.norm() < MIN_DIS) {
//                 if (goback == true)reach=true;
//                 if (enough==true)reach=true;

//                 if(ball_status == -1)
//                     goto help_control_3;
//                 else if(ball_status == 0)
//                     goto help_control_4;
//                 else if (ball_status == 1)
//                     goto help_control_5;
//                 //goto judge;
//             }
//         }
//         action{
//             WalkToTarget(Pose2f(0.0f, 0.4f, 0.0f), Pose2f(0.0f, theBallModel.estimate.position.x(), 0.0f));//if cannot walk straight,change angle

//         }
//     }

//     state(help_control_3)
//     {
//         transition
//         {
//             if(state_time > 1300)//ok for -1
//                 goto judge;
//         }
//         action{
//             WalkToTarget(Pose2f(0.3f, 0.0f, 0.0f), Pose2f(1.0f, 0, 0.0f));//if cannot walk straight,change angle
//         }
//     }

//     state(help_control_4)
//     {
//         transition
//         {
//             if(state_time > 1300)
//                 goto judge;
//         }
//         action{
//             WalkToTarget(Pose2f(0.3f, 0.0f, 0.0f), Pose2f(1.0f, 0, 0.0f));//if cannot walk straight,change angle
//         }
//     }
//     state(help_control_5)
//     {
//         transition
//         {
//             if(state_time > 1300)
//                 goto judge;
//         }
//         action{
//             WalkToTarget(Pose2f(0.3f, 0.0f, 0.0f), Pose2f(1.0f, 0, 0.0f));//if cannot walk straight,change angle
//         }
//     }

//     state(judge) //adjust self position to keep ball.y,obstacle.y around +-100.f(which avoids kick out the door)
//     {

//         transition
//         {
//             if (state_time > 1000)
//             {
//                 int count = theObstacleModel.obstacles.size();
//                 if (count != 0)
//                 {
//                     if (goback == false)//true
//                     {
//                         int i=1,j=0;
//                             center_y=theObstacleModel.obstacles[j].center.y();
//                             while(i != count)
//                             {
//                                 cy=theObstacleModel.obstacles[i].center.y();
//                                 if(std::fabs(cy)<std::fabs(center_y)){
//                                     j=i;
//                                     center_y=theObstacleModel.obstacles[i].center.y();
//                                 }
//                                 i++;
//                             }
//                             center_y = theObstacleModel.obstacles[j].center.y();
//                             left_y = theObstacleModel.obstacles[j].left.y();
//                             right_y = theObstacleModel.obstacles[j].right.y();
//                             width = left_y-right_y;

//                         if (left_y < 0 - (1.5*width)) 
//                         {
                            
//                                 PlaySound("notPenalized.wav");

//                                 if(ball_status == -1)
//                                 {
//                                     time_up = 1.0f;
//                                     goto judge_l_or_r;
//                                 }
//                                 else if(ball_status == 0)
//                                 {
//                                     time_up = 0.8f;
//                                     goto turnLeft;
//                                 }
//                                 else
//                                 {
//                                     time_up = 1.0f;
//                                     goto judge_l_or_r;
//                                 }

//                             } 
//                         else if (right_y > (1.5*width)) {
                            
//                                 PlaySound("penalized.wav");

//                                 if(ball_status == -1)
//                                 {
//                                     time_up =0.4f;
//                                     angle=small_angle*0.3;//ok
//                                     angle_turn=(float(width/7000.0));//changed in 7.23
//                                     //goto turnLeft;
//                                     goto judge_l_or_r;
//                                 }
//                                 else if(ball_status == 0)
//                                 {
//                                     // time_up=0.5f;
//                                     // goto turnRight;
//                                     //goto kickForward;
//                                     if(ball_before == 1)//left
//                                     {
//                                         time_up = 0.5;
//                                         angle_turn = 10;
//                                         goto turnLeft;
//                                     }
//                                     else if(ball_before == -1)
//                                     {
//                                         time_up=0.5;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                     else
//                                     {
//                                         time_up=0.8;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                 }
//                                 else
//                                 {
//                                     time_up=1.0f;
//                                     //goto turnLeft;
//                                     goto judge_l_or_r;
//                                 }
//                             } 
//                         else if (left_y>-(width/2.0)&& left_y <(width/2.0)) {
                            
//                                 angle = small_angle;
//                                 PlaySound("doh.wav");
//                                 if(ball_status == 1)
//                                 {
//                                     // time_up =1.0f;
//                                     // angle_turn=(float(width/1000.0))*1.09;//ok
//                                     // goto turnLeft;
//                                     goto judge_l_or_r;
//                                 }
//                                 else if(ball_status == -1)
//                                 {
//                                     angle_turn=(float(-width/1000.0))*0.8;
//                                     time_up = 0.1;
//                                     //goto turnRight;
//                                     goto judge_l_or_r;
//                                 }
//                                 else if(ball_status == 0)
//                                 {
//                                     if(ball_before == 1)//left
//                                     {
//                                         time_up = 0.5;
//                                         angle_turn = 10;
//                                         goto turnLeft;
//                                     }
//                                     else if(ball_before == -1)
//                                     {
//                                         time_up=0.5;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                     else
//                                     {
//                                         time_up=0.8;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                 }

//                             } 
//                         else if (right_y > -(width/2.0) && right_y < (width/2.0)) 
//                         {
//                             angle = small_angle;
//                             PlaySound("high.wav");    
//                             if(ball_status == 1)
//                             {
//                                 time_up = 1.0f;
//                                 angle_turn=(float(-width/1000.0))*0.35;//ok-middle//ok-left
//                                 //goto turnRight;
//                                 goto judge_l_or_r;
//                             }
//                             else if(ball_status == -1)
//                             {
//                                 //ookk
//                                 time_up = 0.8;
//                                 angle_turn=(float(width/1000.0))*1.09;//ok-725x/...; 
//                                 //goto turnLeft;
//                                 goto judge_l_or_r;
//                             }
                                    
//                             else if(ball_status == 0)
//                             {
//                                 //time_up = 0.5f;
//                                 //angle_turn=(float(-width/1000.0))*0.69;
//                                 //if(signal == ball_on_left_before)
//                                 //goto turnLeft,time_up
//                                 //goto turnRight;
//                                 if(ball_before == 1)//left
//                                     {
//                                         time_up = 0.5;
//                                         angle_turn = 10;
//                                         goto turnLeft;
//                                     }
//                                     else if(ball_before == -1)
//                                     {
//                                         time_up=0.5;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                     else
//                                     {
//                                         time_up=0.8;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                             }  //ok

//                         } 
//                         else if (left_y < 0) 
//                         {
//                             PlaySound("helpMe.wav");
//                             if(ball_status == -1)
//                             {
//                                 time_up =0.1f;
//                                 angle_turn =20;
//                                 goto turnLeft;
//                             }
//                             else if(ball_status == 0)
//                             {
//                                 // time_up = 1.0f;
//                                 // angle_turn = 10;
//                                 // goto turnLeft;//left_y between 0 and -THE_BIG
//                                 //goto kickForward;
//                                 if(ball_before == 1)//left
//                                     {
//                                         time_up = 0.5;
//                                         angle_turn = 10;
//                                         goto turnLeft;
//                                     }
//                                     else if(ball_before == -1)
//                                     {
//                                         time_up=0.5;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                     else
//                                     {
//                                         time_up=0.8;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                             }
//                             else if(ball_status == 1)
//                             {
//                                 time_up = 0.1f;
//                                 angle_turn=-10;
//                                 goto turnLeft;

//                             }

//                         } 
//                         else 
//                         {
//                             PlaySound("lost.wav");
//                             if(ball_status == -1)
//                             {
//                                 //ookk
//                                 time_up = 0.2;
//                                 angle_turn = -5;
//                                 goto turnRight;
//                             }
//                             else if(ball_status == 0)
//                             {
//                                 if(ball_before == 1)//left
//                                     {
//                                         time_up = 0.5;
//                                         angle_turn = 10;
//                                         goto turnLeft;
//                                     }
//                                     else if(ball_before == -1)
//                                     {
//                                         time_up=0.5;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                     else
//                                     {
//                                         time_up=0.8;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                             }
//                             else
//                             {
//                                 goto judge_l_or_r;
//                             }
//                         }              
//                     }


//                     //======================main part=============================
//                     //============================================================

//                     else if(goback == false)
//                     {

//                             int i=1,j=0;
//                             center_y=theObstacleModel.obstacles[j].center.y();
//                             while(i != count){
//                                 cy=theObstacleModel.obstacles[i].center.y();
//                                 if(std::fabs(cy)<std::fabs(center_y)){
//                                     j=i;
//                                     center_y=theObstacleModel.obstacles[i].center.y();
//                                 }
//                                 i++;
//                             }
//                             center_y = theObstacleModel.obstacles[j].center.y();
//                             left_y = theObstacleModel.obstacles[j].left.y();
//                             right_y = theObstacleModel.obstacles[j].right.y();
//                             width = left_y-right_y;


//                         if(Op_keeper == 0)
//                             width=width*1.2;
//                          else if(Op_keeper == 1)
//                             width = width*1.3;

//                         if (left_y < 0 - (1.5*width)) 
//                         {
                            
//                                 PlaySound("notPenalized.wav");

//                                 if(ball_status == -1)
//                                 {
//                                     time_up = 1.0f;
//                                     goto judge_l_or_r;
//                                 }
//                                 else if(ball_status == 0)
//                                 {
//                                     time_up = 0.8f;
//                                     goto turnLeft;
//                                 }
//                                 else
//                                 {
//                                     time_up = 1.0f;
//                                     goto judge_l_or_r;
//                                 }

//                             } 
//                         else if (right_y > (1.5*width)) {
                            
//                                 PlaySound("penalized.wav");

//                                 if(ball_status == -1)
//                                 {
//                                     time_up =0.4f;
//                                     angle=small_angle*0.3;//ok
//                                     angle_turn=(float(width/7000.0));//changed in 7.23
//                                     //goto turnLeft;
//                                     goto judge_l_or_r;
//                                 }
//                                 else if(ball_status == 0)
//                                 {
//                                     // time_up=0.5f;
//                                     // goto turnRight;
//                                     //goto kickForward;
//                                     if(ball_before == 1)//left
//                                     {
//                                         time_up = 0.5;
//                                         angle_turn = 10;
//                                         goto turnLeft;
//                                     }
//                                     else if(ball_before == -1)
//                                     {
//                                         time_up=0.5;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                     else
//                                     {
//                                         time_up=0.8;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                 }
//                                 else
//                                 {
//                                     time_up=1.0f;
//                                     //goto turnLeft;
//                                     goto judge_l_or_r;
//                                 }
//                             } 
//                         else if (left_y>-(width/2.0)&& left_y <(width/2.0)) {
                            
//                                 angle = small_angle;
//                                 PlaySound("doh.wav");
//                                 if(ball_status == 1)
//                                 {
//                                     // time_up =1.0f;
//                                     // angle_turn=(float(width/1000.0))*1.09;//ok
//                                     // goto turnLeft;
//                                     goto judge_l_or_r;
//                                 }
//                                 else if(ball_status == -1)
//                                 {
//                                     angle_turn=(float(-width/1000.0))*0.8;
//                                     time_up = 0.1;
//                                     //goto turnRight;
//                                     goto judge_l_or_r;
//                                 }
//                                 else if(ball_status == 0)
//                                 {
//                                     if(ball_before == 1)//left
//                                     {
//                                         time_up = 0.5;
//                                         angle_turn = 10;
//                                         goto turnLeft;
//                                     }
//                                     else if(ball_before == -1)
//                                     {
//                                         time_up=0.5;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                     else
//                                     {
//                                         time_up=0.8;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                 }

//                             } 
//                         else if (right_y > -(width/2.0) && right_y < (width/2.0)) 
//                         {
//                             angle = small_angle;
//                             PlaySound("high.wav");    
//                             if(ball_status == 1)
//                             {
//                                 time_up = 1.0f;
//                                 angle_turn=(float(-width/1000.0))*0.35;//ok-middle//ok-left
//                                 //goto turnRight;
//                                 goto judge_l_or_r;
//                             }
//                             else if(ball_status == -1)
//                             {
//                                 //ookk
//                                 time_up = 0.8;
//                                 angle_turn=(float(width/1000.0))*1.09;//ok-725x/...; 
//                                 //goto turnLeft;
//                                 goto judge_l_or_r;
//                             }
                                    
//                             else if(ball_status == 0)
//                             {
//                                 //time_up = 0.5f;
//                                 //angle_turn=(float(-width/1000.0))*0.69;
//                                 //if(signal == ball_on_left_before)
//                                 //goto turnLeft,time_up
//                                 //goto turnRight;
//                                 if(ball_before == 1)//left
//                                     {
//                                         time_up = 0.5;
//                                         angle_turn = 10;
//                                         goto turnLeft;
//                                     }
//                                     else if(ball_before == -1)
//                                     {
//                                         time_up=0.5;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                     else
//                                     {
//                                         time_up=0.8;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                             }  //ok

//                         } 
//                         else if (left_y < 0) 
//                         {
//                             PlaySound("helpMe.wav");
//                             if(ball_status == -1)
//                             {
//                                 time_up =0.1f;
//                                 angle_turn =20;
//                                 goto turnLeft;
//                             }
//                             else if(ball_status == 0)
//                             {
//                                 // time_up = 1.0f;
//                                 // angle_turn = 10;
//                                 // goto turnLeft;//left_y between 0 and -THE_BIG
//                                 //goto kickForward;
//                                 if(ball_before == 1)//left
//                                     {
//                                         time_up = 0.5;
//                                         angle_turn = 10;
//                                         goto turnLeft;
//                                     }
//                                     else if(ball_before == -1)
//                                     {
//                                         time_up=0.5;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                     else
//                                     {
//                                         time_up=0.8;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                             }
//                             else if(ball_status == 1)
//                             {
//                                 time_up = 0.1f;
//                                 angle_turn=-10;
//                                 goto turnLeft;

//                             }

//                         } 
//                         else 
//                         {
//                             PlaySound("lost.wav");
//                             if(ball_status == -1)
//                             {
//                                 //ookk
//                                 time_up = 0.2;
//                                 angle_turn = -5;
//                                 goto turnRight;
//                             }
//                             else if(ball_status == 0)
//                             {
//                                 if(ball_before == 1)//left
//                                     {
//                                         time_up = 0.5;
//                                         angle_turn = 10;
//                                         goto turnLeft;
//                                     }
//                                     else if(ball_before == -1)
//                                     {
//                                         time_up=0.5;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                     else
//                                     {
//                                         time_up=0.8;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                             }
//                             else
//                             {
//                                 goto judge_l_or_r;
//                             }
//                         }
//                     }



//                     if (backtimes < 4)
//                     {
//                             helpx=helpx + 10;
//                             helpy=helpy + 10;
//                             goto Goback2judge; 
//                         }
//                     else
//                     {
//                         if (enough == false){
//                             enough= true;
//                             goto walkToball;
//                         }else{

//                             if(ball_status == 0)
//                             {
//                                 if(ball_before == 1)//left
//                                     {
//                                         time_up = 0.5;
//                                         angle_turn = 10;
//                                         goto turnLeft;
//                                     }
//                                     else if(ball_before == -1)
//                                     {
//                                         time_up=0.5;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                                     else
//                                     {
//                                         time_up=0.8;
//                                         angle_turn = -10;
//                                         goto turnRight;
//                                     }
//                             }
//                             else
//                                 goto judge_l_or_r;
//                             //goto judge_l_or_r;
//                         }                                     
//                     }

//                 }//if count != 0
//             }//if state_time

//         }//transition

//         action{
//                 theHeadControlMode = HeadControl::searchObs;
//                 Stand();
//             }
//     }

//     state(turnRight) {//move to the position after play allright sound
      

//         transition{
//             if (theBallModel.estimate.position.norm() > 200) {
//                 if(state_time > 2500*time_up)
//                 goto judge_l_or_r;
//             }
//             else
//                 goto judge_l_or_r;
//         }
//         action{
//             PlaySound("jump.wav");
//             WalkToTarget(Pose2f(-0.5f, 0.05f, 0.05f), Pose2f(angle_turn, MIN_DIS*(1.0f - float(cos(angle))), -MIN_DIS * float(sin(angle)+10)));//improve
            
//         }
//     }

//     state(turnLeft) {
       

//         transition{
//             if (theBallModel.estimate.position.norm() > 200) {
//                 if(state_time > 2500*time_up)
//                 goto judge_l_or_r;
//             }
//             else
//                 goto judge_l_or_r;
//         }
//         action{
//             PlaySound("allright.wav");
//             WalkToTarget(Pose2f(0.5f, 0.05f, 0.05f), Pose2f(angle_turn, MIN_DIS* (1.0f - float(cos(angle))), MIN_DIS* float(sin(angle)-10)));
            
//         }
//     }
//     state(kickForward) {
//         transition{
//             if (action_done)
//                 goto finish;
//         }
//         action{
//             KickForward();
//         }
//     }

//     state(kickForward_left) {
//         transition{
//             if (action_done)
//                 goto finish;
//         }
//         action{
//             KickForward_left();
//         }
//     }

//     state(finish) {
//         transition{
//             if (state_time > 3000){
//                 enough=false;
//                 goback=false;
//                 backtimes=0;
//                 angle_turn=0;
//             }
//         }
//         action{
//             theHeadControlMode = HeadControl::searchForBall;
//             Stand();
//         }
//     }

//     state(judge_l_or_r){
//         transition{
//             ball_x = theBallModel.estimate.position.x();
//             ball_y = theBallModel.estimate.position.y();
//             if (ball_y > 0){
//                 goto kickleft;
//             }
//             else {
//                 goto kickright;
//             }


//         }
//         action{
//             Stand();
//             theHeadControlMode = HeadControl::searchObs;

//             ball_x = theBallModel.estimate.position.x();
//             ball_y = theBallModel.estimate.position.y();

//             center_y = theObstacleModel.obstacles[0].center.y();
//             left_y = theObstacleModel.obstacles[0].left.y();
//             right_y = theObstacleModel.obstacles[0].right.y();
//         }
//     }

//     state(kickleft){
//         transition{
//             if(action_done)
//                 goto finish;
//         }
//         action{
            
//             KickLeft();
//         }
//     }

//     state(kickright){
//         transition{
//             if (action_done){
//                 goto finish;
//             }
//         }
//         action{
//             KickRight();
//         }
//     }



//     state(Goback2judge){
//         transition{
//             if(state_time > 2000){
//                 backtimes = backtimes+1;
//                 goback=true;
//                 goto judge;
//             }
//         }
//         action{
//             WalkToTarget(Pose2f(0.f,0.5f,0.f),Pose2f(0.f,-100.f-helpx,0.f));
//         }
//     }

}

    




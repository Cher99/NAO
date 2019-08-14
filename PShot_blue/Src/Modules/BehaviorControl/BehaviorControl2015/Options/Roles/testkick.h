#define TEST_TURN -1
#define MIN_DIS (300.0f)

float time_up_t = 0;
float angle_turn_t = 0;



option(testkick) {

    //WalkToTarget(Pose2f(0.5f, 0.05f, 0.05f), Pose2f(10, 10, 10)); turn to right
    //printf("%lf\n",theBallModel.estimate.position.angle());//value in rad.



    //=====================BALL IN Midlle=====================
    //========================================================
    //================= time_up_t param adjust ===============shi jian zeng yi 
    
    // float angle_2 = 0.52f;
    // initial_state(test_AngleTurn_0)
    // {
    //     transition
    //     {
    //         if(TEST_TURN == 1)//turn left;
    //         {
    //             time_up_t = 0.8;
    //             angle_turn_t = -10;
    //         }
    //         else if(TEST_TURN == 0)
    //         {
    //             time_up_t = 1.0;
    //             angle_turn_t = 10;
    //         }
    //         else if(TEST_TURN == -1)
    //         {
    //             time_up_t= 0.8;//or 0.8
    //             angle_turn_t = 10;
    //         }
    //         goto test_AngleTurn_1;
    //     }
    //     action
    //     {
    //         ;
    //     }
    // }
    // state(test_AngleTurn_1)
    // {
    //     transition
    //     {
    //             if(state_time > 2500*time_up_t)
    //             goto stand;
    //     }
    //     action
    //     {
    //         //need to adjust;
    //         WalkToTarget(Pose2f(0.5f, 0.5f, 0.4f), Pose2f(angle_turn, MIN_DIS*(1.0f - float(cos(angle_2))), MIN_DIS * float(sin(angle_2))));    
    //     }
    // }
    // target_state(stand)
    // {
    //     transition
    //     {
    //         ;
    //     }
    //     action
    //     {
    //         Stand();
    //     }
    // }





    //=======================alignBehindBall===================
    //=========================================================
    //=============param tested:                   ============
    //===========1.in time 2s,alignWalkSpeed x/y ==============
    //===========2.ball_before_left:turn_back_angle============
    //===========3.ball_before_middle: turn_back_angle=========
    //===========4.ball_before_rigth: turn_back_angle =========
    // initial_state(lauch)
    // {
    //     transition
    //     {
    //         if(state_time > 2000)
    //             goto stand;
    //     } 
    //     action
    //     {
    //         WalkToTarget(Pose2f(0.0f,0,50.0f),Pose2f(0,0,150));
    //     }
    // }
    // target_state(stand)
    // {
    //     transition
    //     {
    //         ;
    //     }
    //     action
    //     {
    //         Stand();
    //     }
    // }



    //====================Plan B====================
    /*
        params different with A :angle_2
                                time_up_t
                                turnSpeedPose2f

    */
    // float angle_2 = 1.04f; ///     changed!
    // initial_state(test_AngleTurn_0)
    // {
    //     transition
    //     {
    //         if(TEST_TURN == 1)//turn right;
    //         {
    //             time_up_t = 0.8;
    //             angle_turn_t = -10;
    //         }
    //         else if(TEST_TURN == 0)
    //         {
    //             time_up_t = 1.0;
    //             angle_turn_t = 10;
    //         }
    //         else if(TEST_TURN == -1)//turn left
    //         {
    //             time_up_t= 1.9;//or 0.8
    //             angle_turn_t = -10;
    //         }
    //         goto test_AngleTurn_1;
    //     }
    //     action
    //     {
    //         ;
    //     }
    // }
    // state(test_AngleTurn_1)
    // {
    //     transition
    //     {
    //             if(state_time > 2500*time_up_t)
    //             goto stand;
    //     }
    //     action
    //     {
    //         //PLAN B has different params with A !!!
    //         WalkToTarget(Pose2f(0.7f, 0.25f, 0.35f), Pose2f(angle_turn, MIN_DIS*(1.0f - float(cos(angle_2))), -MIN_DIS * float(sin(angle_2))));    
    //     }
    // }
    // target_state(stand)
    // {
    //     transition
    //     {
    //         ;
    //     }
    //     action
    //     {
    //         Stand();
    //     }
    // }


    //================test kickLeft//kickRight ===============
    //kick foward by left leg
    // initial_state(start) {
    //     transition{
    //         if (theBallModel.estimate.position.y()> 45 && theBallModel.estimate.position.y()< 55 
    //             && theBallModel.estimate.position.x()> 150 && theBallModel.estimate.position.y()< 170 )
    //             goto kick;
    //     }
    //     action{
    //         theHeadControlMode = HeadControl::lookDown;

    //         WalkToTarget(Pose2f(0.0f,0.3f,0.3f),Pose2f(0.f,theBallModel.estimate.position.x()-160.f,theBallModel.estimate.position.y()-50.f));
    //         //WalkToTarget(Pose2f(0.0f, 0.3f, 0.3f),
    //         //Pose2f(0.f, theBallModel.estimate.position.x() +160.f, theBallModel.estimate.position.y() + 55.f));//cyj:walk to right so that all is rightly before left leg.
    //         //the paramerter is set radically
    //     }
    // }
    // state(kick) {
    //     transition{
    //         if (action_done  && state_time > 1000)
    //             goto finish;
    //     }
    //     action{
    //         //PlaySound("Nao.wav");
    //         theHeadControlMode = HeadControl::lookDown;
    //         Kick(KickRequest::BQ_Kick, true);//BQ_kick_small
    //         //InWalkKick(WalkRequest::left, Pose2f(libCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));

    //     }

    // }
    // target_state(finish) {
    //     transition{
    //         ;
    //     }
    //     action{
    //         theHeadControlMode = HeadControl::lookDown;
    //         Stand();
    //     }
    // }



    // initial_state(start) {
    //     transition{ 
    //         if (theBallModel.estimate.position.y()< -50 && theBallModel.estimate.position.y()> -60 
    //             && theBallModel.estimate.position.x()> 150 && theBallModel.estimate.position.y()< 170)
    //             goto kick;
    //     }
    //     action{
    //         theHeadControlMode = HeadControl::lookDown;
    //         WalkToTarget(Pose2f(0.0f, 0.3f, 0.3f),
    //         Pose2f(0.f, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() + 55.f));
    //         //the paramerter is set radically
    //     }
    // }
    // state(kick) {
    //     transition{
    //         if (action_done  && state_time > 1000)
    //             goto finish;
    //     }
    //     action{
    //         PlaySound("arm.wav");
    //         theHeadControlMode = HeadControl::lookDown;
    //         Kick(KickRequest::BQ_Kick, false);//BQ_Kick_small
    //         //InWalkKick(WalkRequest::left, Pose2f(libCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));

    //     }

    // }
    // target_state(finish) {
    //     transition{
    //         ;
    //     }
    //     action{
    //         theHeadControlMode = HeadControl::lookDown;
    //         Stand();
    //     }
    // }


    // initial_state(launch)
    // {
    //     transition
    //     {
    //         if(theBallModel.estimate.position.norm() < 300)
    //             goto stand;
    //     }
    //     action
    //     {
    //         WalkToTarget(Pose2f(0.0f, 0.4f, 0.0f), Pose2f(0.0f, theBallModel.estimate.position.x(), 0.0f));//if cannot walk straight,change angle
    //     }
    // }
    // state(stand)
    // {
    //     transition
    //     {
    //         ;
    //     }
    //     action
    //     {
    //         Stand();
    //     }
    // }

    //======================================
    //======================================
    initial_state(launch)
    {
        
        transition
        {
            //5cm ? have great influence on which leg to kick after walking 50cm.
            if(theBallModel.estimate.position.y()<50 && theBallModel.estimate.position.y()>-50)
                goto stand;
        }
        action
        {
            //if angle turned too big or small,change angle speed.
            WalkToTarget(Pose2f(-0.05f, 0.05f, 0.5f), Pose2f(1.f, 0.0f, theBallModel.estimate.position.y()));
        }
    }
    state(stand)
    {
        transition
        {
            ;
        }
        action
        {
            Stand();
        }
    }


}



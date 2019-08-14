option(LeftKick2) {
    initial_state(start) {
        transition{
            if ( theBallModel.estimate.position.norm() < 300.f &&
            libCodeRelease.between(  theBallModel.estimate.position.y(), -10.f, 110.f ) )
                goto judge;
        }
        action{
            theHeadControlMode  = HeadControl::lookDown;
            WalkToTarget( Pose2f(0.3f, 0.3f, 0.3f),
            Pose2f(0.03f, theBallModel.estimate.position.x() - 200.f,
            theBallModel.estimate.position.y() + 55.f)) ;
        }
    }
    state(judge) {
        transition{
            if (state_time > 2000) {
                unsigned i = 0;
                for ( i = 0; i < theObstacleModel.obstacles.size(); i++) {
                    //if(theObstacleModel.obstacles[i].type == Obstacle::unknown)
                    {
                        if (theObstacleModel.obstacles[i].center.y() >  -100.f  )
                            goto rightKick;
                    }
                }
                goto kick;
            }
        }
        action{
            theHeadControlMode = HeadControl::searchObs;
            Stand();
        }
    }
    state(kick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            theHeadControlMode = HeadControl::lookDown;
            KickForward();
        }
    }
    state(rightKick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            KickToRight();
        }
    }
    target_state(finish) {
        transition{
            ;
        }
        action{
            theHeadControlMode = HeadControl::lookForward;
            Stand();
        }
    }
}
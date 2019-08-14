option(MiddleKick2) {
    initial_state(start) {
        transition{
            if (theBallModel.estimate.position.norm() < 250.f &&
            libCodeRelease.between(theBallModel.estimate.position.y(), -60.f, 60.f ) )
                goto judge;
        }
        action{
            theHeadControlMode  = HeadControl::lookDown;
            WalkToTarget( Pose2f(0.15f, 0.15f, 0.15f),
            Pose2f(0.0f, theBallModel.estimate.position.x() - 180.f,
            theBallModel.estimate.position.y()));
        }
    }
    state(judge) {
        transition{
            if (state_time > 5000) {
                unsigned i = 0;
                for ( i = 0; i < theObstacleModel.obstacles.size(); i++) {
                    //if(theObstacleModel.obstacles[i].type == Obstacle::unknown)
                    {

                        float  y = theObstacleModel.obstacles[i].center.y();
                        if (y >= 0.f )
                            goto rightKick;
                        else if (y < 0.f )
                            goto leftKick;
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
    state(rightKick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            KickToRight();
        }
    }
    state(leftKick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{

            KickToLeft();
        }
    }
    state(kick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            KickForward();
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
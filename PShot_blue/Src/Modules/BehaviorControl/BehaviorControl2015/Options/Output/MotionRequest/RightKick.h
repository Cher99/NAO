option(RightKick) {
    initial_state(start) {
        transition{
            if ( theBallModel.estimate.position.norm() < 500.f && libCodeRelease.between(  theBallModel.estimate.position.y(), -100.f, -10.f ) )
                goto judge;
        }
        action{
            theHeadControlMode  = HeadControl::searchForBall;
            WalkToTarget(Pose2f(0.3f, 0.3f, 0.3f), Pose2f(0.f, theBallModel.estimate.position.x() - 300.f, theBallModel.estimate.position.y() - 55.f)) ;
        }
    }
    state(avoid) {
        transition{
            if (state_time > 3000)
                goto judge;
        }
        action{
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(0.3f, 0.3f, 0.3f), Pose2f(0.2f, theBallModel.estimate.position.x() - 240.f, theBallModel.estimate.position.y() - 100.f  ) );
        }
    }

    /*返回0 说明球门柱不在视野的 you 边，
    返回1视野的左边有一个门柱，且障碍物在you   zheng 前方，
    返回2视野的左边有一个门柱，且障碍物在 zuo 前方，
    */
    state(judge) {
        transition{
            if (state_time > 3000) {
                int flag = IfKickRight(theObstacleModel, theGoalPercept);
                if (flag == 0)
                    goto avoid;
                else if (flag == 1)
                    goto leftKick;
                else if (flag == 2 )
                    goto kick;
            }
        }
        action{
            theHeadControlMode = HeadControl::lookUp;
            Stand();
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
    state(leftKick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            KickToLeft();
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
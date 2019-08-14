option(KickForward) {
    initial_state(start) {
        transition{
            if (theBallModel.estimate.position.y()> -70 && theBallModel.estimate.position.y()< -60 
                && theBallModel.estimate.position.x()> 150 && theBallModel.estimate.position.y()< 170)
                goto kick;
        }
        action{
            theHeadControlMode = HeadControl::lookDown;
            WalkToTarget(Pose2f(0.0f, 0.3f, 0.3f),
            Pose2f(0.f, theBallModel.estimate.position.x() - 155.f, theBallModel.estimate.position.y() + 65.f));
            //the paramerter is set radically
        }
    }
    state(kick) {
        transition{
            if (action_done  && state_time > 1000)
                goto finish;
        }
        action{
            PlaySound("heat.wav");
            theHeadControlMode = HeadControl::lookDown;
            Kick(KickRequest::BQ_Kick, false);//BQ_kick_small
            //InWalkKick(WalkRequest::left, Pose2f(libCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));

        }

    }
    target_state(finish) {
        transition{
            ;
        }
        action{
            theHeadControlMode = HeadControl::lookDown;
            Stand();
        }
    }
}
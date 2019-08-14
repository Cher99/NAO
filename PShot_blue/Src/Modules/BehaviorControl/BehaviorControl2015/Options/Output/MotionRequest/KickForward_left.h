option(KickForward_left) {
    initial_state(start) {
        transition{
            if (action_done && state_time > 2000)
                goto kick;
        }
        action{
            theHeadControlMode = HeadControl::lookDown;
            WalkToTarget(Pose2f(0.3f, 0.3f, 0.3f),
            Pose2f(-0.05f, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() + 55.f));
            //after turn right,it will adjust its position to a little left and kick (r or l?)
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
            Kick(KickRequest::BQ_Kick, false);
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
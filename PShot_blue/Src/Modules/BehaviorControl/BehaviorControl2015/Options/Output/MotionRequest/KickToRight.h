option(KickToRight) {
    initial_state(start) {
        transition{
            if (state_time > 4000 )
                goto kick;
        }
        action{
            theHeadControlMode = HeadControl::lookDown;
            WalkToTarget(Pose2f(0.3f, 0.3f, 0.3f) ,
            Pose2f( 0.f , theBallModel.estimate.position.x() - 160.f,
            theBallModel.estimate.position.y() - 15.f )  ) ;
        }
    }
    state(kick) {
        transition{
            if (action_done  && state_time > 1000)
                goto finish;
        }
        action{
            theHeadControlMode = HeadControl::lookDown;
            Kick(KickRequest:: rs_kick, true);
            PlaySound("allright.wav");
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
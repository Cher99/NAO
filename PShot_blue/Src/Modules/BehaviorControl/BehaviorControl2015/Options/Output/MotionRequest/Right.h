option(Right) {
    initial_state(start) {
        transition{
            if (theBallModel.estimate.position.norm() < 400.f   &&   libCodeRelease.between(  theBallModel.estimate.position.y() , -80.f, -20.f   ) )
                goto  turn;
        }
        action {
            theHeadControlMode = HeadControl::searchForBall;
            WalkToTarget( Pose2f(0.3f, 0.3f, 0.7f), Pose2f(0.f, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y()  - 50.f)  );
        }
    }
    state(turn ) {
        transition {
            if (state_time > 3000)
                goto lightmove;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(0.3f, 0.3f, 0.3f), Pose2f(10_deg, theBallModel.estimate.position.x() - 300.f , theBallModel.estimate.position.y() - 75.f ) ) ;
        }
    }
    state(lightmove) {
        transition{
            if (state_time > 4000  )
                goto kick;
        }
        action{
            theHeadControlMode = HeadControl::lookForward;
            WalkToTarget(Pose2f(0.3f, 0.3f, 0.3f), Pose2f(0.f, theBallModel.estimate.position.x() - 150.f , theBallModel.estimate.position.y() - 65.f ) ) ;
        }
    }
    state(kick) {
        transition{
            if (state_time > 3000)
                goto finish;
        }
        action{
            theHeadControlMode = HeadControl::lookForward;
            Kick(KickRequest::BL_Kick, true);

        }
    }
    target_state(finish) {
        transition{
            ;
        }
        action{
            Stand();
        }
    }
}

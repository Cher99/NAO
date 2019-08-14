option(Left) {
    initial_state(start) {
        transition{
            if (theBallModel.estimate.position.norm() < 400.f   &&   libCodeRelease.between(  theBallModel.estimate.position.y() ,  40,  70.f   ) )
                goto  turn;
        }
        action {
            theHeadControlMode = HeadControl::searchForBall;
            WalkToTarget( Pose2f(0.3f, 0.3f, 0.6f), Pose2f(0.f, theBallModel.estimate.position.x() - 300.f , theBallModel.estimate.position.y() + 50.f )  );
        }
    }
    state(turn ) {
        transition {
            if (state_time > 3000   )
                goto lightmove;
        }
        action {
            theHeadControlMode = HeadControl :: lookForward;
            WalkToTarget(Pose2f(0.3f, 0.3f, 0.4f), Pose2f(-0.2f, theBallModel.estimate.position.x() - 240.f , theBallModel.estimate.position.y() + 70.f ) ) ;
        }
    }
    state(lightmove) {
        transition{
            if (state_time > 8000)
                goto kick;
        }
        action{
            theHeadControlMode = HeadControl :: lookForward;
            WalkToTarget(Pose2f(0.3f, 0.3f, 0.3f), Pose2f(0.f, theBallModel.estimate.position.x() - 160.f , theBallModel.estimate.position.y() + 55.f ) ) ;
        }
    }
    state(kick) {
        transition{
            if (state_time > 3000)
                goto finish;
        }
        action{
            theHeadControlMode = HeadControl::lookForward;
            Kick(KickRequest::BL_Kick, false);

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

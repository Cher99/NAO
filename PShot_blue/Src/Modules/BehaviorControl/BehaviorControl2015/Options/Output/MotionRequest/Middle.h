option(Middle) {
    initial_state(start) {
        transition{
            if (theBallModel.estimate.position.norm() < 300.f  &&  libCodeRelease.between(  theBallModel.estimate.position.y() , -20.f, 20.f   ) )
                goto  turn;
        }
        action {
            theHeadControlMode = HeadControl::searchForBall;
            WalkToTarget( Pose2f(0.3f, 0.3f, 0.3f), Pose2f(0.05f , theBallModel.estimate.position.x() - 300.f, theBallModel.estimate.position.y() )  );
        }
    }
    state(turn ) {
        transition {
            if (state_time > 3000  )
                goto turn1;
        }
        action{
            theHeadControlMode = HeadControl :: lookForward;
            WalkToTarget( Pose2f(0.2f, 0.2f, 0.2f), Pose2f( 0.08f,  theBallModel.estimate.position.x() - 240.f ,  theBallModel.estimate.position.y() - 55.f )  );

        }

    }
    state(turn1) {
        transition{
            if (state_time > 3000)
                goto kick;
        }
        action{
            theHeadControlMode = HeadControl :: lookForward;
            WalkToTarget(Pose2f(0.2f, 0.2f, 0.2f), Pose2f(0.f,  theBallModel.estimate.position.x() - 160.f ,  theBallModel.estimate.position.y() + 55.f )  );
        }
    }
    state(kick) {
        transition {
            if (state_time > 2000)
                goto finish;
        }
        action {
            theHeadControlMode = HeadControl::lookForward;
            Kick(KickRequest::BQ_Kick, false);

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
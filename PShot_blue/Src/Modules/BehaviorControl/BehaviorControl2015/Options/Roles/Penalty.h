option(Penalty) {
    initial_state(start) {
        transition {
            if (libCodeRelease.timeSinceBallWasSeen () < 1000) {
                if (theBallModel.estimate.position.y() > 500.f)
                    goto	Left;
                else if (theBallModel.estimate.position.y() < -500.f)
                    goto 	Right;
                else
                    goto	Middle;
            }
        }
        action {
            theHeadControlMode = HeadControl::searchForBall;
            Stand();
        }
    }

    state(Left) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            Left();
        }
    }
    state(Right) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            Right();
        }
    }
    state(Middle) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            Middle();
        }
    }
    state(finish) {
        transition{
            if (state_time > 5000)
                goto start;
        }
        action{
            Stand();
            theHeadControlMode = HeadControl::lookForward;
        }
    }

}
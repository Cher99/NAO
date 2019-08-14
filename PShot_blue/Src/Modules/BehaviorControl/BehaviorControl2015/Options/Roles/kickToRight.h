option(kickToRight) {
    initial_state(start) {
        transition{
            if (libCodeRelease.timeSinceBallWasSeen() < 500     ) {
                goto middleKick;
            }
        }
        action{
            theHeadControlMode = HeadControl::searchForBall;
            Stand();
        }
    }
    state(middleKick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            MiddleKick3();
        }
    }


    state(finish) {
        transition{
            if (state_time > 5000)
                goto start;
        }
        action{
            Stand();
        }
    }
}
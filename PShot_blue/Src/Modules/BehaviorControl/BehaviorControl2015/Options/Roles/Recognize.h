int  i = 1;
option(Recognize) {
    initial_state(start) {
        transition{
            if (libCodeRelease.timeSinceBallWasSeen() < 1000     && state_time > 3000) {
                if (theBallModel.estimate.position.y() > 300.f)
                    goto leftKick;
                else if (theBallModel.estimate.position.y() < -300.f)
                    goto rightKick;
                else goto middleKick;
            }
        }
        action{
            i = 1;
            theHeadControlMode = HeadControl::searchForBall;
            Stand();
        }
    }
    state(leftKick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            while (i) {
                PlaySound("lost");
                i = 0;
            }
            LeftKick();
        }
    }
    state(rightKick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{

            while (i) {
                PlaySound("allright");
                i = 0;
            }
            RightKick();
        }
    }
    state(middleKick) {
        transition{
            if (action_done)
                goto finish;
        }
        action{
            //while(i)
            {
                PlaySound("Nao");
                i = 0;
            }
            MiddleKick();
        }
    }
    state(finish) {
        transition{
            if (state_time > 3000)
                goto start;
        }
        action{
            theHeadControlMode = HeadControl::searchForBall;
            Stand();
        }
    }
}

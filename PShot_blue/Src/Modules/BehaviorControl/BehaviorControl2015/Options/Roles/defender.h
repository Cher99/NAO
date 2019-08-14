    
option(defender) {
    initial_state(lauch)
    {
        transition
        {
            if(theBallModel.estimate.position.y() > -7 && theBallModel.estimate.position.y() < 7)
            {
                //goto start;
                goto  gap;
            }
            else if(state_time > 2000)
            {
                //goto start;
                goto sitDown;
            }
        }
        action
        {
             WalkToTarget(Pose2f(0.05f, 0.1f, 0.5f), Pose2f(theBallModel.estimate.position.angle(), theBallModel.estimate.position.x(), theBallModel.estimate.position.y()));
        
        }
    }
    state(gap)
    {
        transition
        {
            if(state_time > 500)
                goto start;
        }
        action
        {
            Stand();
        }
    }

    state(start) {
        transition{
            if (theBallModel.estimate.velocity.norm() != 0 )//relative speed?
                goto sitDown;
        }
        action{
            theHeadControlMode = HeadControl::lookForward;
            Stand();
        }
    }

    state(sitDown) {
        transition {
            if (false)
                goto getUp;
        }
        action {
            if (true)
                SpecialAction(SpecialActionRequest::keeperDown,  true);
            else
                SpecialAction(SpecialActionRequest::keeperDown,  false);
            theHeadControlMode = HeadControl::lookForward;

        }
    }

    state(getUp) {
        transition {
            if (action_done && state_time > 4000)
                goto start ;
        }
        action {
            //SpecialAction(SpecialActionRequest::keeperUp , false);
            //GetUpEngine();
            Stand();
            theHeadControlMode = HeadControl::lookForward;
        }
    }
}

/*
option(defender) {

    initial_state(start) {
        transition{
            if (state_time > 5000)
                goto sitDown;
        }
        action{
            theHeadControlMode = HeadControl::lookForward;
            Stand();
        }
    }

    state(sitDown) {
        transition {
            if (action_done && state_time > 1000000)
                goto getUp;
        }
        action {
            if (theBallModel.estimate.position.y() > 0.f)
                SpecialAction(SpecialActionRequest::keeperDown,  true);
            else
                SpecialAction(SpecialActionRequest::keeperDown,  false);
            theHeadControlMode = HeadControl::lookForward;

        }
    }

    state(getUp) {
        transition {
            if (action_done && state_time > 2000)
                goto start ;
        }
        action {
            //SpecialAction(SpecialActionRequest::keeperUp , false);
            //GetUpEngine();
            Stand();
            theHeadControlMode = HeadControl::lookForward;
        }
    }
}*/

/* ======demo=======
option(defender) 
{
    state(start) {
        transition{
            if (state_time > 5000)
                goto sitDown;
        }
        action{
            theHeadControlMode = HeadControl::lookForward;
            Stand();
        }
    }

    state(sitDown) {
        transition {
            if (action_done && state_time > 1000000)
                goto getUp;
        }
        action {
            if (theBallModel.estimate.position.y() > 0.f)
                SpecialAction(SpecialActionRequest::keeperDown,  true);
            else
                SpecialAction(SpecialActionRequest::keeperDown,  false);
            theHeadControlMode = HeadControl::lookForward;

        }
    }

    state(getUp) {
        transition {
            if (action_done && state_time > 2000)
                goto start ;
        }
        action {
            //SpecialAction(SpecialActionRequest::keeperUp , false);
            //GetUpEngine();
            Stand();
            theHeadControlMode = HeadControl::lookForward;
        }
    }
}

*/
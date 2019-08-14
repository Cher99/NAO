

/*
penalty kick:Once the Op-Striker toach the Ball,kick is considered down.
*/


option(defender_justStand){
	initial_state(lookAtBall)
	{
		transition{
			if(theBallModel.estimate.velocity.norm() != 0)
				goto alarm;
		}
		action{
			FocusBall();
            Stand();
		}
	}

	state(alarm)
	{
		transition{
			if(theBallModel.estimate.position.x()<300)
			goto kickAway;
		}
		action{
			FocusBall();
			if(std::abs(theBallModel.estimate.velocity.x()) < 80 && theBallModel.estimate.position.x()>300)
				WalkToTarget(Pose2f(0.05f, 0.0f, 0.7f), Pose2f(theBallModel.estimate.position.angle(), 0.0f, theBallModel.estimate.position.y()));
			else 
				WalkToTarget(Pose2f(0.05f, 0.1f, 0.7f), Pose2f(theBallModel.estimate.position.angle(), theBallModel.estimate.position.x(), theBallModel.estimate.position.y()));
		}
	}
	state(kickAway) {
        transition{
            if (action_done && state_time > 2000)
                goto kick;
        }
        action{
            theHeadControlMode = HeadControl::lookDown;
            if(theBallModel.estimate.velocity.x() < 50)
            {
	            WalkToTarget(Pose2f(0.0f, 0.5f, 0.5f),
	            Pose2f(0.f, theBallModel.estimate.position.x() - 155.f, theBallModel.estimate.position.y() + 55.f));
            }
            else
            {
            	WalkToTarget(Pose2f(0.0f, 0.7f, 0.7f),
	            Pose2f(0.f, theBallModel.estimate.position.x() - 155.f, theBallModel.estimate.position.y() + 55.f));
            }
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
    state(finish) {
        transition{
            ;
        }
        action{
            theHeadControlMode = HeadControl::lookDown;
            Stand();
        }
    }
}
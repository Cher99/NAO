option(MiddleKick3)
{
	initial_state(start){
		transition{
			if( theBallModel.estimate.position.norm() < 250.f && 
				libCodeRelease.between(  theBallModel.estimate.position.y(), -60.f,60.f ) )
				goto rightKick;
		}	
		action{
			theHeadControlMode  = HeadControl::lookDown;
			WalkToTarget( Pose2f(0.3f,0.3f,0.3f),
				Pose2f(0.025f,theBallModel.estimate.position.x() -180.f, 
					theBallModel.estimate.position.y()    )) ;
		}
	}
	
	state(rightKick){
		transition{
			if(action_done)
				goto finish;
		}
		action{
			KickToRight();
		}
	}
	
	target_state(finish){
		transition{
			;
		}
		action{
			theHeadControlMode =HeadControl::lookForward;
			Stand();
		}
	}
}
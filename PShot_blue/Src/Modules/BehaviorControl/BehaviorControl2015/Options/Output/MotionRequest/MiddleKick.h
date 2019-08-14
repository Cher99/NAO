option(MiddleKick)
{
	initial_state(start){
		transition{
			if( theBallModel.estimate.position.norm() < 400.f && libCodeRelease.between(  theBallModel.estimate.position.y(), -30.f,30.f ) )
				goto judge;
		}	
		action{
			theHeadControlMode  = HeadControl::searchForBall;
			WalkToTarget( Pose2f(0.4f,0.4f,0.4f),Pose2f(0.f,theBallModel.estimate.position.x() -300.f, theBallModel.estimate.position.y()   )) ;
		}
	}
/*
在球门正前方，只判断障碍物，根据障碍物的位置，选择侧踢的动作。
返回0 向右侧踢
返回1 向左侧踢
*/
	state(judge){
		transition{
			if(state_time > 3000) {
			int flag=IfKickMiddle(theObstacleModel );
			if(  flag ==0 )
				goto  rightKick;
			else if(flag ==1)
				goto leftKick;
			}
		}
		action{
			theHeadControlMode = HeadControl::lookUp;
			Stand();
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
	state(leftKick){
		transition{
			if(action_done)
				goto finish;
		}
		action{
			KickToLeft();
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
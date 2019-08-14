/** All option files that belong to the current behavior have to be included by this file. */

#include "Options/Soccer.h"

#include "Options/GameControl/HandleGameState.h"
#include "Options/GameControl/HandlePenaltyState.h"
#include "Options/GameControl/PlayingState.h"
#include "Options/GameControl/ReadyState.h"

#include "Options/HeadControl/HeadControl.h"
#include "Options/HeadControl/LookForward.h"
#include "Options/HeadControl/SearchForBall.h"
#include "Options/HeadControl/FocusBall.h"
#include "Options/HeadControl/LookUp.h"
#include "Options/HeadControl/LookDown.h"
#include "Options/HeadControl/SearchObs.h"

#include "Options/Output/Annotation.h"
#include "Options/Output/ArmMotionRequest/KeyFrameArms.h"

#include "Options/Output/HeadMotionRequest/SetHeadPanTilt.h"
#include "Options/Output/HeadMotionRequest/SetHeadTarget.h"
#include "Options/Output/HeadMotionRequest/SetHeadTargetOnGround.h"

#include "Options/Output/MotionRequest/GetUpEngine.h"
#include "Options/Output/MotionRequest/InWalkKick.h"
#include "Options/Output/MotionRequest/PathToTarget.h"
#include "Options/Output/MotionRequest/SpecialAction.h"
#include "Options/Output/MotionRequest/Stand.h"
#include "Options/Output/MotionRequest/WalkAtSpeed.h"
#include "Options/Output/MotionRequest/WalkAtSpeedPercentage.h"
#include "Options/Output/MotionRequest/WalkToTarget.h"
#include "Options/Output/MotionRequest/Kick.h"
#include "Options/Output/MotionRequest/Right.h"
#include "Options/Output/MotionRequest/Left.h"
#include "Options/Output/MotionRequest/Middle.h"
#include "Options/Output/MotionRequest/LeftKick.h"
#include "Options/Output/MotionRequest/RightKick.h"
#include "Options/Output/MotionRequest/MiddleKick.h"
#include "Options/Output/MotionRequest/KickForward.h"
#include "Options/Output/MotionRequest/KickForward_left.h"
#include "Options/Output/MotionRequest/KickToRight.h"
#include "Options/Output/MotionRequest/KickToLeft.h"
#include "Options/Output/MotionRequest/LeftKick2.h"
#include "Options/Output/MotionRequest/RightKick2.h"
#include "Options/Output/MotionRequest/MiddleKick2.h"
#include "Options/Output/MotionRequest/MiddleKick3.h"
#include "Options/Output/MotionRequest/ShowBallModel.h"

//-----------------add----------------------------
#include "Options/Output/MotionRequest/KickLeft.h"
#include "Options/Output/MotionRequest/KickRight.h"

#include "Options/Output/debughelp/ShowPosiBL2f.h"
#include "Options/Output/debughelp/ShowPosiOb3y.h"
//-----------------end----------------------------


#include "Options/Output/PlaySound.h"

#include "Options/Roles/Striker.h"
#include "Options/Roles/Penalty.h"
#include "Options/Roles/Recognize.h"
#include "Options/Roles/Obstacle.h"
#include "Options/Roles/testkick.h"
#include "Options/Roles/kickToRight.h"
#include "Options/Roles/defender.h"
#include "Options/Roles/defender_justStand.h"
#include "Options/Roles/penaltyKickerAngle.h"
#include "Options/Roles/penaltyKickerAngle_2.h"
#include "Options/Roles/penaltyKickerAngleLeft.h"
#include "Options/Roles/penaltyKickerAngleRight.h"
#include "Options/Roles/walkStraight.h"

#include "Options/Skills/ArmContact.h"
#include "Options/Skills/GetUp.h"

#include "Options/Tools/ButtonPressedAndReleased.h"

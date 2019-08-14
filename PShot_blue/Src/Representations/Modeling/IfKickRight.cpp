#include "BallModel.h"
#include "Tools/Modeling/Obstacle.h"
#include "Tools/Math/Eigen.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"
#include "ObstacleModel.h"
#include  "Representations/Perception/GoalPercept.h"
#include "IfKickRight.h"
/*返回0 说明球门柱不在视野的右边，
返回1视野的左边有一个门柱，且障碍物在右前方或正前方，
返回2视野的左边有一个门柱，且障碍物在左前方
*/
int  IfKickRight(const ObstacleModel& theObstacle,  const GoalPercept& theGoalPercept   )
{
    if(theGoalPercept.goalPosts.size() ==0)
        return 0;
    else if(theGoalPercept.goalPosts.size() ==1)
    {
        if(theGoalPercept.goalPosts[0].positionInImage.x() <160.f )
            return 0;
    }
    else if(theGoalPercept.goalPosts.size() ==2)
     if(theGoalPercept.goalPosts[0].positionInImage.x() < 160.f &&  theGoalPercept.goalPosts[1].positionInImage.x() < 160.f )
        return 0;


    if(   (theGoalPercept.goalPosts.size() == 1 && theGoalPercept.goalPosts[0].positionInImage.x()>= 160.f )   ||
         (theGoalPercept.goalPosts.size() == 2 && 
           (theGoalPercept.goalPosts[0].positionInImage.x()>= 160.f || theGoalPercept.goalPosts[1].positionInImage.x() > 160.f   ) ) )
    {
        unsigned i=0;
        for( i =0; i < theObstacle.obstacles.size();i++)
        {
            if(theObstacle.obstacles[i].type == Obstacle::unknown)
            {
                if(theObstacle.obstacles[i].center.y() <  300.f )
                    return 1;
                if(theObstacle.obstacles[i].center.y() > 300.f)
                    return 2;
            }
            
        }
    }
return 0;
}
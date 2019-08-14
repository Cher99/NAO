#include "BallModel.h"
#include "Tools/Modeling/Obstacle.h"
#include "Tools/Math/Eigen.h"
#include "RobotPose.h"
#include "Tools/Math/Pose2f.h"
#include "ObstacleModel.h"
#include "IfKickMiddle.h"
/*
在球门正前方，只判断障碍物，根据障碍物的位置，选择侧踢的动作。
返回0 向右侧踢
返回1 向左侧踢
*/
int  IfKickMiddle(const ObstacleModel & theObstacle  )
{
   
    
        unsigned i=0;
        for( i =0; i < theObstacle.obstacles.size();i++)
        {
            if(theObstacle.obstacles[i].type == Obstacle::unknown)
            {
                if(theObstacle.obstacles[i].center.y() >  0.f )
                    return 0;
                if(theObstacle.obstacles[i].center.y() <= 0.f)
                    return 1;
            }
            
        
    }
    return 0;
}
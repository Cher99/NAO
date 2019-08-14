float angle11 = 0.5f;
option(walkStraight) {
    initial_state(start) {
        transition{
            if (state_time > 1000) {
                goto walkToBall1;
            }
        }
        action{
            WalkToTarget(Pose2f(0.3f, 0.4f, 0.0f), Pose2f(0.35f, 10000.0f, 0.0f));
            //WalkToTarget(Pose2f(0.3f, 0.2f, 0.2f), Pose2f(0.5f, 0.0f, 0.0f));
            //WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), Pose2f(0 - angle1, 200 * (1.0f - float(cos(angle1))), 200.0f * float(sin(angle1))));
        }
    }

    state(walkToBall1) {
        transition{
            if (theBallModel.estimate.position.norm() < 200.0f) {
                goto turnLeft;
            }
        }
        action{
            WalkToTarget(Pose2f(0.f, 0.3f, 0.3f), Pose2f(0.0f, theBallModel.estimate.position.x(), theBallModel.estimate.position.y()));
        }
    }

    state(turnLeft) {
        transition{
            if (theBallModel.estimate.position.norm() < 200.0f) {
                goto finish;
            }
        }
        action{
            WalkToTarget(Pose2f(0.5f, 0.3f, 0.3f), Pose2f(0.1f, 0.0f, 0.0f));
        }
    }

    state(finish) {
        transition{
            if (state_time > 3000)
                ;
        }
        action{
            KickForward();
            //Stand();
        }
    }
}

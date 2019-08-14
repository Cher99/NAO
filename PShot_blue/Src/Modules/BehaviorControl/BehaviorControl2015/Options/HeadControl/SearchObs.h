/** Pun intended. Looks straight ahead in a way that the Nao V4's cameras cover the area in front of its feet as well as everything else in front of the robot.*/
option(SearchObs, (float) (0.45f) tilt) {
    /** Simply sets the necessary angles */
    initial_state(lookLeft) {
        transition{
            if (state_time > 1000)
                goto lookRight;
        }
        action {
            SetHeadPanTilt(-5_deg, tilt, 5_deg);
        }
    }
    state(lookRight) {
        transition{
            if (state_time > 2000)
                goto lookLeft2;
        }
        action {
            SetHeadPanTilt(5_deg, tilt, 5_deg);
        }
    }
    state(lookLeft2) {
        transition{
            if (state_time > 2000)
                goto lookRight;
        }
        action {
            SetHeadPanTilt(-5_deg, tilt, 5_deg);
        }
    }

}
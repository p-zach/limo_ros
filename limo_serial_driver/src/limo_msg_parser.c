
#include "limo_message.h"
#include "limo_protocol_parser.h"
#include "limo_msg_parser.h"


bool DecodeCanFrame(const struct can_frame *rx_frame, AgxMessage *msg){

    switch (rx_frame->can_id) {
    /***************** command frame *****************/
    case CAN_MSG_MOTION_COMMAND_ID: {
      msg->type = AgxMsgMotionCommand;
      // parse frame buffer to message
      MotionCommandFrame *frame = (MotionCommandFrame *)(rx_frame->data);
      msg->body.motion_command_msg.linear_velocity =
          (int16_t)((uint16_t)(frame->linear_velocity.low_byte) |
                    (uint16_t)(frame->linear_velocity.high_byte) << 8) /
          1000.0;
      msg->body.motion_command_msg.angular_velocity =
          (int16_t)((uint16_t)(frame->angular_velocity.low_byte) |
                    (uint16_t)(frame->angular_velocity.high_byte) << 8) /
          1000.0;
      msg->body.motion_command_msg.lateral_velocity =
          (int16_t)((uint16_t)(frame->lateral_velocity.low_byte) |
                    (uint16_t)(frame->lateral_velocity.high_byte) << 8) /
          1000.0;
      msg->body.motion_command_msg.steering_angle =
          (int16_t)((uint16_t)(frame->steering_angle.low_byte) |
                    (uint16_t)(frame->steering_angle.high_byte) << 8) /
          1000.0;
      break;
    }
    case CAN_MSG_LIGHT_COMMAND_ID: {
      msg->type = AgxMsgLightCommand;
      // parse frame buffer to message
      LightCommandFrame *frame = (LightCommandFrame *)(rx_frame->data);
      msg->body.light_command_msg.cmd_ctrl_allowed =
          (frame->cmd_ctrl_allowed != 0) ? true : false;
      msg->body.light_command_msg.front_light.mode = frame->front_light_mode;
      msg->body.light_command_msg.front_light.custom_value =
          frame->front_light_custom;
      msg->body.light_command_msg.rear_light.mode = frame->rear_light_mode;
      msg->body.light_command_msg.rear_light.custom_value =
          frame->rear_light_custom;
      break;
    }
    case CAN_MSG_BRAKING_COMMAND_ID: {
      msg->type = AgxMsgBrakingCommand;
      // TODO
      break;
    }
    /***************** feedback frame ****************/
    case CAN_MSG_SYSTEM_STATE_ID: {
      msg->type = AgxMsgSystemState;
      SystemStateFrame *frame = (SystemStateFrame *)(rx_frame->data);
      msg->body.system_state_msg.vehicle_state = frame->vehicle_state;
      msg->body.system_state_msg.control_mode = frame->control_mode;
      msg->body.system_state_msg.battery_voltage =
          (int16_t)((uint16_t)(frame->battery_voltage.low_byte) |
                    (uint16_t)(frame->battery_voltage.high_byte) << 8) *
          0.1;
      msg->body.system_state_msg.error_code =
          (uint16_t)(frame->error_code.low_byte) |
          (uint16_t)(frame->error_code.high_byte) << 8;
      msg->body.system_state_msg.motion_mode = frame->motion_mode;
      break;
    }
    case CAN_MSG_MOTION_STATE_ID: {
      msg->type = AgxMsgMotionState;
      MotionStateFrame *frame = (MotionStateFrame *)(rx_frame->data);
      msg->body.motion_state_msg.linear_velocity =
          (int16_t)((uint16_t)(frame->linear_velocity.low_byte) |
                    (uint16_t)(frame->linear_velocity.high_byte) << 8) /
          1000.0;
      msg->body.motion_state_msg.angular_velocity =
          (int16_t)((uint16_t)(frame->angular_velocity.low_byte) |
                    (uint16_t)(frame->angular_velocity.high_byte) << 8) /
          1000.0;
      msg->body.motion_state_msg.lateral_velocity =
          (int16_t)((uint16_t)(frame->lateral_velocity.low_byte) |
                    (uint16_t)(frame->lateral_velocity.high_byte) << 8) /
          1000.0;
      msg->body.motion_state_msg.steering_angle =
          (int16_t)((uint16_t)(frame->steering_angle.low_byte) |
                    (uint16_t)(frame->steering_angle.high_byte) << 8) /
          1000.0;
      break;
    }
    case CAN_MSG_LIGHT_STATE_ID: {
      msg->type = AgxMsgLightState;
      LightStateFrame *frame = (LightStateFrame *)(rx_frame->data);
      msg->body.light_command_msg.cmd_ctrl_allowed =
          (frame->cmd_ctrl_allowed != 0) ? true : false;
      msg->body.light_command_msg.front_light.mode = frame->front_light_mode;
      msg->body.light_command_msg.front_light.custom_value =
          frame->front_light_custom;
      msg->body.light_command_msg.rear_light.mode = frame->rear_light_mode;
      msg->body.light_command_msg.rear_light.custom_value =
          frame->rear_light_custom;
      break;
    }
    case CAN_MSG_RC_STATE_ID: {
      msg->type = AgxMsgRcState;
      RcStateFrame *frame = (RcStateFrame *)(rx_frame->data);
      // switch a
      if ((frame->sws & RC_SWA_MASK) == RC_SWA_UP_MASK)
        msg->body.rc_state_msg.swa = RC_SWITCH_UP;
      else if ((frame->sws & RC_SWA_MASK) == RC_SWA_DOWN_MASK)
        msg->body.rc_state_msg.swa = RC_SWITCH_DOWN;
      // switch b
      if ((frame->sws & RC_SWB_MASK) == RC_SWB_UP_MASK)
        msg->body.rc_state_msg.swb = RC_SWITCH_UP;
      else if ((frame->sws & RC_SWB_MASK) == RC_SWB_MIDDLE_MASK)
        msg->body.rc_state_msg.swb = RC_SWITCH_MIDDLE;
      else if ((frame->sws & RC_SWB_MASK) == RC_SWB_DOWN_MASK)
        msg->body.rc_state_msg.swb = RC_SWITCH_DOWN;
      // switch c
      if ((frame->sws & RC_SWC_MASK) == RC_SWC_UP_MASK)
        msg->body.rc_state_msg.swc = RC_SWITCH_UP;
      else if ((frame->sws & RC_SWC_MASK) == RC_SWC_MIDDLE_MASK)
        msg->body.rc_state_msg.swc = RC_SWITCH_MIDDLE;
      else if ((frame->sws & RC_SWC_MASK) == RC_SWC_DOWN_MASK)
        msg->body.rc_state_msg.swc = RC_SWITCH_DOWN;
      // switch d
      if ((frame->sws & RC_SWD_MASK) == RC_SWD_UP_MASK)
        msg->body.rc_state_msg.swd = RC_SWITCH_UP;
      else if ((frame->sws & RC_SWD_MASK) == RC_SWD_DOWN_MASK)
        msg->body.rc_state_msg.swd = RC_SWITCH_DOWN;
      msg->body.rc_state_msg.stick_right_v = frame->stick_right_v;
      msg->body.rc_state_msg.stick_right_h = frame->stick_right_h;
      msg->body.rc_state_msg.stick_left_v = frame->stick_left_v;
      msg->body.rc_state_msg.stick_left_h = frame->stick_left_h;
      msg->body.rc_state_msg.var_a = frame->var_a;
      break;
    }
    case CAN_MSG_ACTUATOR1_HS_STATE_ID:
    case CAN_MSG_ACTUATOR2_HS_STATE_ID:
    case CAN_MSG_ACTUATOR3_HS_STATE_ID:
    case CAN_MSG_ACTUATOR4_HS_STATE_ID:
    case CAN_MSG_ACTUATOR5_HS_STATE_ID:
    case CAN_MSG_ACTUATOR6_HS_STATE_ID:
    case CAN_MSG_ACTUATOR7_HS_STATE_ID:
    case CAN_MSG_ACTUATOR8_HS_STATE_ID: {
      msg->type = AgxMsgActuatorHSState;
      ActuatorHSStateFrame *frame = (ActuatorHSStateFrame *)(rx_frame->data);
      msg->body.actuator_hs_state_msg.motor_id =
          rx_frame->can_id - CAN_MSG_ACTUATOR1_HS_STATE_ID;
      msg->body.actuator_hs_state_msg.rpm =
          (int16_t)((uint16_t)(frame->rpm.low_byte) |
                    (uint16_t)(frame->rpm.high_byte) << 8);
      msg->body.actuator_hs_state_msg.current =
          ((uint16_t)(frame->current.low_byte) |
           (uint16_t)(frame->current.high_byte) << 8) *
          0.1;
      msg->body.actuator_hs_state_msg.pulse_count =
          (int32_t)((uint32_t)(frame->pulse_count.lsb) |
                    (uint32_t)(frame->pulse_count.low_byte) << 8 |
                    (uint32_t)(frame->pulse_count.high_byte) << 16 |
                    (uint32_t)(frame->pulse_count.msb) << 24);
      break;
    }
    case CAN_MSG_ACTUATOR1_LS_STATE_ID:
    case CAN_MSG_ACTUATOR2_LS_STATE_ID:
    case CAN_MSG_ACTUATOR3_LS_STATE_ID:
    case CAN_MSG_ACTUATOR4_LS_STATE_ID:
    case CAN_MSG_ACTUATOR5_LS_STATE_ID:
    case CAN_MSG_ACTUATOR6_LS_STATE_ID:
    case CAN_MSG_ACTUATOR7_LS_STATE_ID:
    case CAN_MSG_ACTUATOR8_LS_STATE_ID: {
      msg->type = AgxMsgActuatorLSState;
      ActuatorLSStateFrame *frame = (ActuatorLSStateFrame *)(rx_frame->data);
      msg->body.actuator_hs_state_msg.motor_id =
          rx_frame->can_id - CAN_MSG_ACTUATOR1_LS_STATE_ID;
      msg->body.actuator_ls_state_msg.driver_voltage =
          ((uint16_t)(frame->driver_voltage.low_byte) |
           (uint16_t)(frame->driver_voltage.high_byte) << 8) *
          0.1;
      msg->body.actuator_ls_state_msg.driver_temperature =
          (int16_t)((uint16_t)(frame->driver_temperature.low_byte) |
                    (uint16_t)(frame->driver_temperature.high_byte) << 8);
      msg->body.actuator_ls_state_msg.motor_temperature =
          frame->motor_temperature;
      msg->body.actuator_ls_state_msg.driver_state = frame->driver_state;
      break;
    }
    case CAN_MSG_CURRENT_CTRL_MODE:
    {
      msg->type=AgxMsgMotionModeState;
      MotionModeStateFrame *frame = (MotionModeStateFrame*)(rx_frame->data);

      msg->body.motion_mode_feedback_msg.motion_mode = frame->motion_mode;
      msg->body.motion_mode_feedback_msg.mode_changing = frame->mode_changing;
      // printf("mode: %d, changing: %d\n", frame->motion_mode, frame->mode_changing);
      break;
    }
    /****************** sensor frame *****************/
    case CAN_MSG_ODOMETRY_ID: {
      msg->type = AgxMsgOdometry;
      OdometryFrame *frame = (OdometryFrame *)(rx_frame->data);
      msg->body.odometry_msg.left_wheel =
          (int32_t)((uint32_t)(frame->left_wheel.lsb) |
                    (uint32_t)(frame->left_wheel.low_byte) << 8 |
                    (uint32_t)(frame->left_wheel.high_byte) << 16 |
                    (uint32_t)(frame->left_wheel.msb) << 24);
      msg->body.odometry_msg.right_wheel =
          (int32_t)((uint32_t)(frame->right_wheel.lsb) |
                    (uint32_t)(frame->right_wheel.low_byte) << 8 |
                    (uint32_t)(frame->right_wheel.high_byte) << 16 |
                    (uint32_t)(frame->right_wheel.msb) << 24);
      break;
    }
    case CAN_MSG_IMU_ACCEL_ID: { // accelerate
      msg->type = AgxMsgImuAccel;
      ImuAccelFrame* frame = (ImuAccelFrame*)(rx_frame->data);
      msg->body.imu_accel_msg.accel_x = ((int16_t)((int16_t)frame->accel_x.high_byte << 8 | frame->accel_x.low_byte)) / 100.0;
      msg->body.imu_accel_msg.accel_y = ((int16_t)((int16_t)frame->accel_y.high_byte << 8 | frame->accel_y.low_byte)) / 100.0;
      msg->body.imu_accel_msg.accel_z = ((int16_t)((int16_t)frame->accel_z.high_byte << 8 | frame->accel_z.low_byte)) / 100.0;
      break;
    }
    case CAN_MSG_IMU_GYRO_ID: {
      msg->type = AgxMsgImuGyro;
      ImuGyroFrame *frame = (ImuGyroFrame *)(rx_frame->data);
      msg->body.imu_gyro_msg.gyro_x = ((int16_t)((int16_t)frame->gyro_x.high_byte << 8 | frame->gyro_x.low_byte)) / 100.0;
      msg->body.imu_gyro_msg.gyro_y = ((int16_t)((int16_t)frame->gyro_y.high_byte << 8 | frame->gyro_y.low_byte)) / 100.0;
      msg->body.imu_gyro_msg.gyro_z = ((int16_t)((int16_t)frame->gyro_z.high_byte << 8 | frame->gyro_z.low_byte)) / 100.0;
      break;
    }
    case CAN_MSG_IMU_EULER_ID: {
      msg->type = AgxMsgImuEuler;
      ImuEulerFrame *frame = (ImuEulerFrame *)(rx_frame->data);
      msg->body.imu_euler_msg.yaw =    ((int16_t)((int16_t)frame->yaw.high_byte << 8 | frame->yaw.low_byte)) / 100.0;
      msg->body.imu_euler_msg.pitch =  ((int16_t)((int16_t)frame->pitch.high_byte << 8 | frame->pitch.low_byte)) / 100.0;
      msg->body.imu_euler_msg.roll =   ((int16_t)((int16_t)frame->roll.high_byte << 8 | frame->roll.low_byte)) / 100.0;
      break;
    }
    case CAN_MSG_SAFETY_BUMPER_ID: {
      msg->type = AgxMsgSafetyBumper;
      // TODO
      break;
    }
    case CAN_MSG_ULTRASONIC_1_ID:
    case CAN_MSG_ULTRASONIC_2_ID:
    case CAN_MSG_ULTRASONIC_3_ID:
    case CAN_MSG_ULTRASONIC_4_ID:
    case CAN_MSG_ULTRASONIC_5_ID:
    case CAN_MSG_ULTRASONIC_6_ID:
    case CAN_MSG_ULTRASONIC_7_ID:
    case CAN_MSG_ULTRASONIC_8_ID: {
      msg->type = AgxMsgUltrasonic;
      // TODO
      break;
    }
    case CAN_MSG_UWB_1_ID:
    case CAN_MSG_UWB_2_ID:
    case CAN_MSG_UWB_3_ID:
    case CAN_MSG_UWB_4_ID: {
      msg->type = AgxMsgUwb;
      // TODO
      break;
    }
    case CAN_MSG_BMS_BASIC_ID: {
      msg->type = AgxMsgBmsBasic;
      // TODO
      break;
    }
    case CAN_MSG_BMS_EXTENDED_ID: {
      msg->type = AgxMsgBmsExtended;
      // TODO
      break;
    }
    /*************** query/config frame **************/
    case CAN_MSG_VERSION_REQUEST_ID: {
      msg->type = AgxMsgVersionRequest;
      // TODO
      break;
    }
    case CAN_MSG_VERSION_RESPONSE_ID: {
      msg->type = AgxMsgVersionResponse;
      // TODO
      break;
    }
    case CAN_MSG_CTRL_MODE_CONFIG_ID: {
      msg->type = AgxMsgControlModeConfig;
      ControlModeConfigFrame *frame =
          (ControlModeConfigFrame *)(rx_frame->data);
      msg->body.control_mode_config_msg.mode = frame->mode;
      break;
    }
    case CAN_MSG_STEER_NEUTRAL_REQUEST_ID: {
      msg->type = AgxMsgSteerNeutralRequest;
      // TODO
      break;
    }
    case CAN_MSG_STEER_NEUTRAL_RESPONSE_ID: {
      msg->type = AgxMsgSteerNeutralResponse;
      // TODO
      break;
    }
    case CAN_MSG_STATE_RESET_CONFIG_ID: {
      msg->type = AgxMsgStateResetConfig;
      StateResetConfigFrame *frame = (StateResetConfigFrame *)(rx_frame->data);
      msg->body.state_reset_config_msg.error_clear_byte =
          frame->error_clear_byte;
      break;
    }
    default:
      break;
  }
}
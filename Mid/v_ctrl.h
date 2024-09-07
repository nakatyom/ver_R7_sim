#ifndef _VELOCITY_CONTROL_H
#define _VELOCITY_CONTROL_H

/* extern functions */
extern void mid_velocity_control(float velo_str_tgt, float velo_rot_tgt);
extern void mid_velocity_control_str(float velo_str_tgt);
extern void mid_velocity_control_rot(float velo_rot_tgt);

#endif	// _VELOCITY_CONTROL_H
//
// Simple test for the AP_Scheduler interface
//

#include <AP_HAL/AP_HAL.h>
#include <AP_InertialSensor/AP_InertialSensor.h>
#include <AP_Scheduler/AP_Scheduler.h>
#include <AP_AHRS/AP_AHRS.h>
#include <AP_BoardConfig/AP_BoardConfig.h>
#include <AP_Logger/AP_Logger.h>
#include <AP_Baro/AP_Baro.h>
const AP_HAL::HAL& hal = AP_HAL::get_HAL();

AP_Int32 log_bitmask;
AP_Logger AP_Logger{log_bitmask};

class SchedTest {
public:
    void setup();
    void loop();

private:

    AP_InertialSensor ins;
    AP_Scheduler scheduler{nullptr};

    uint32_t ins_counter;
    static const AP_Scheduler::Task scheduler_tasks[];

    void ins_update(void);
    void ahrs_update(void);
    void one_hz_print(void);
    void five_second_call(void);
    void two_second_call(void);
};

static AP_BoardConfig board_config;
static SchedTest schedtest;

#define SCHED_TASK(func, _interval_ticks, _max_time_micros) SCHED_TASK_CLASS(SchedTest, &schedtest, func, _interval_ticks, _max_time_micros)

/*
  scheduler table - all regular tasks are listed here, along with how
  often they should be called (in 20ms units) and the maximum time
  they are expected to take (in microseconds)
 */
const AP_Scheduler::Task SchedTest::scheduler_tasks[] = {
    //SCHED_TASK(ahrs_update,             100,   1000),
    SCHED_TASK(ins_update,             50,   1000),
    SCHED_TASK(one_hz_print,            1,   1000),
    SCHED_TASK(two_second_call,      0.5,   1000),
    SCHED_TASK(five_second_call,      0.2,   1800),
};
class DummyVehicle {
public:
    AP_AHRS_NavEKF ahrs{AP_AHRS_NavEKF::FLAG_ALWAYS_USE_EKF};
};

static DummyVehicle vehicle;

// choose which AHRS system to use
// AP_AHRS_DCM ahrs = AP_AHRS_DCM::create(barometer, gps);
AP_AHRS_NavEKF &ahrs = vehicle.ahrs;

// create barometer object
static AP_Baro barometer;

void SchedTest::setup(void)
{

    board_config.init();
    
    ins.init(scheduler.get_loop_rate_hz());
    ahrs.init();
    //     // initialize the barometer
     barometer.init();
     barometer.calibrate();

    // initialise the scheduler
    scheduler.init(&scheduler_tasks[0], ARRAY_SIZE(scheduler_tasks), (uint32_t)-1);
}

void SchedTest::loop(void)
{
    // run all tasks
    //ahrs.update(true);
    scheduler.loop();
}

/*
  update inertial sensor, reading data 
 */
void SchedTest::ins_update(void)
{
    ins_counter++;
    //ins.update();
    //barometer.accumulate();
}

void SchedTest::ahrs_update(void)
{
    //ahrs.update();
    //barometer.accumulate();
}

/*
  print something once a second
 */
void SchedTest::two_second_call(void) {
  if (!barometer.healthy()) {
    hal.console->printf("not healthy\n");
    return;
  }

  // output barometer readings to console
//   hal.console->printf(
//       " Pressure: %.2f Pa\n"
//       " Temperature: %.2f degC\n"
//       " Relative Altitude: %.2f m\n"
//       " climb=%.2f m/s\n"
//       "\n",
//       (double)barometer.get_pressure(), (double)barometer.get_temperature(),
//       (double)barometer.get_altitude(), (double)barometer.get_climb_rate());
}

void SchedTest::one_hz_print(void)
{
    hal.console->printf("one_hz: t=%lu\n", (unsigned long)AP_HAL::millis());
        Vector3f drift  = ahrs.get_gyro_drift();
    hal.console->printf(
            "r:%4.1f  p:%4.1f y:%4.1f "
                "drift=(%5.1f %5.1f %5.1f)\n",
            (double)ToDeg(ahrs.roll),
            (double)ToDeg(ahrs.pitch),
            (double)ToDeg(ahrs.yaw),
            (double)ToDeg(drift.x),
            (double)ToDeg(drift.y),
            (double)ToDeg(drift.z));
}

/*
  print something every 5 seconds
 */
void SchedTest::five_second_call(void)
{

    hal.console->printf("five_seconds: t=%lu ins_counter=%u\n", (unsigned long)AP_HAL::millis(), (unsigned)ins_counter);
}

/*
  compatibility with old pde style build
 */
void setup(void);
void loop(void);

void setup(void)
{
    schedtest.setup();
}
void loop(void)
{
    
    schedtest.loop();
}
AP_HAL_MAIN();

#pragma once
#include <G3D/G3D.h>
#include "Experiment.h"
#include "App.h"
#include "sqlHelpers.h"

class App;

class TargetExperiment : public Experiment
{
protected:
	double m_totalRemainingTime = 0;
	double m_lastMotionChangeAt;
	double m_scoreboardDuration = 10.0; // show the score for at least this amount of seconds.
	
	std::vector<std::vector<std::string>> m_playerActions;
	std::vector<std::vector<std::string>> m_targetTrajectory;
	std::vector<std::vector<std::string>> m_frameInfo;

	double m_lastFireAt = 0.f;

	TargetExperiment(App* app) : Experiment(app) {
		// secure vector capacity large enough so as to avoid memory allocation time.
		m_playerActions.reserve(5000);
		m_targetTrajectory.reserve(5000);
	}

public:
	static shared_ptr<TargetExperiment> create(App* app) {
		return createShared<TargetExperiment>(app);
	}

	static shared_ptr<TargetExperiment> create_empty(App* app) {
		shared_ptr<TargetExperiment> texp = create(app);
		texp->m_psych.mMeasurements = {};
	}

	/**creates a new target with randomized motion path and gives it to the app */
	void initTargetAnimation();
    virtual double weaponCooldownPercent() override;

	float randSign();

	void updatePresentationState();

	void onInit();

	void onGraphics3D(RenderDevice * rd, Array<shared_ptr<Surface>>& surface);

	void onSimulation(RealTime rdt, SimTime sdt, SimTime idt);

	void onUserInput(UserInput * ui);

	void onGraphics2D(RenderDevice * rd);

	void processResponse();

	void recordTrialResponse();

	void accumulateTrajectories();

	void accumulateFrameInfo(RealTime rdt, float sdt, float idt);

	/** queues action with given name to insert into database when trial completes 
	@param action - one of "aim" "hit" "miss" or "invalid (shots limited by fire rate)" */
	virtual void accumulatePlayerAction(String action) override;

	virtual bool responseReady() override;

	bool initPsychHelper();
};

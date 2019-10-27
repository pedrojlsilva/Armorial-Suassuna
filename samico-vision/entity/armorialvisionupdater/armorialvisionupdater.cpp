#include "armorialvisionupdater.h"

QString ArmorialVisionUpdater::name() {
    return "ArmorialVisionUpdater";
}

ArmorialVisionUpdater::ArmorialVisionUpdater(ArmorialVisionClient *ArmorialVisionClient, FieldAreas::FieldArea fieldLimit, bool enableLossFilter, bool enableKalmanFilter, bool enableNoiseFilter,  bool debugDetection, bool debugGeometry) {
    _ArmorialVisionClient = ArmorialVisionClient;
    _fieldLimit = fieldLimit;
    _debugDetection = debugDetection;
    _debugGeometry = debugGeometry;

    // Create Sensor (GEARSystem)
    _sensor = new Sensor();

    // Initialize robot objects
    QHash<int,Robot*> yellowRobots;
    QHash<int,Robot*> blueRobots;
    for(int id=0; id<MAX_ROBOTS; id++) {
        Robot *yellowRobot = new Robot(Colors::YELLOW, _yellowTeamIndex, id, enableLossFilter, enableKalmanFilter, enableNoiseFilter, debugDetection);
        yellowRobot->setSensor(_sensor);
        yellowRobots.insert(id, yellowRobot);

        Robot* blueRobot = new Robot(Colors::BLUE, _blueTeamIndex, id, enableLossFilter, enableKalmanFilter, enableNoiseFilter, debugDetection);
        blueRobot->setSensor(_sensor);
        blueRobots.insert(id, blueRobot);
    }
    _objRobots.insert(Colors::YELLOW, yellowRobots);
    _objRobots.insert(Colors::BLUE, blueRobots);

    // Initialize ball object
    _objBall = new Ball(enableLossFilter, enableKalmanFilter, enableNoiseFilter, debugDetection);
    _objBall->setSensor(_sensor);



    // Frequency
    this->setLoopFrequency(60); // Hz
}

ArmorialVisionUpdater::~ArmorialVisionUpdater() {
    // Sensor
    delete _sensor;

    // Robot objects
    QList<QHash<int,Robot*> > robots = _objRobots.values();
    for(int i=0; i<robots.size(); i++) {
        QList<Robot*> colorRobots = robots.at(i).values();

        for(int id=0; id<colorRobots.size(); id++)
            delete colorRobots.at(id);
    }

    // Ball object
    delete _objBall;


}

QList<Robot*> ArmorialVisionUpdater::getRobotObjects() const {
    // Generate robots list
    QList<Robot*> yellowRobots = _objRobots.value(Colors::YELLOW).values();
    QList<Robot*> blueRobots = _objRobots.value(Colors::BLUE).values();

    return QList<Robot*>(yellowRobots+blueRobots);
}

Ball* ArmorialVisionUpdater::getBallObject() const {
    return _objBall;
}

SSL_GeometryFieldSize ArmorialVisionUpdater::getFieldGeometry() const {
    return _ArmorialVisionClient->getGeometryData().field();
}

bool ArmorialVisionUpdater::hasDetectionUpdates(int timeout) const {
    return _newDetectionUpdates.tryAcquire(1, timeout);
}

bool ArmorialVisionUpdater::hasGeometryUpdates() const {
    return _newGeometryUpdates.tryAcquire();
}

//************************Começo dos métodos abstratos de Entity*********************

void ArmorialVisionUpdater::initialization() {

    // WRBackbone connect (as Sensor)
    _sensor->connect("127.0.0.1", 0);
    if(_sensor->isConnected())
        cout << ">> ArmorialVision: Connected to WRBackbone.\n";
    else {
        cout << ">> ArmorialVision: [ERROR] Cannot connect to WRBackbone.\n";
        this->stopRunning();
        return;
    }

    // Sensor configuration
    _sensor->addBall(0);
    _sensor->addTeam(0, "Warthog Robotics");
    _sensor->addTeam(1, "Opponent");
    std::cout << ">> ArmorialVision: Teams #0 (yellow) and #1 (blue) created.\n";
    for(int i=0; i<MAX_ROBOTS; i++) {
        _sensor->addPlayer(0, i);
        _sensor->addPlayer(1, i);
        std::cout << ">> ArmorialVision: Player #" << i << " created.\n";
    }

    // Debug config
    cout.setf(ios::fixed,ios::floatfield);
    cout.precision(2);
}

void ArmorialVisionUpdater::loop() {

    // Detection data
    if(_ArmorialVisionClient->hasDetectionUpdate()) {

        // Request data to EyeClient
        QList<SSL_DetectionFrame> detectionFrames = _ArmorialVisionClient->getDetectionData();

        // Process
        processDetectionData(detectionFrames);

        // Release update
        if(_newDetectionUpdates.available()==0)
            _newDetectionUpdates.release();
    }

    // Geometry data
    if(_ArmorialVisionClient->hasGeometryUpdate()) {

        // Request data to EyeClient
        SSL_GeometryData geometryData = _ArmorialVisionClient->getGeometryData();

        // Process
        processGeometryData(geometryData);

        // Release update
        if(_newGeometryUpdates.available()==0)
            _newGeometryUpdates.release();
    }
}

void ArmorialVisionUpdater::finalization() {
    _sensor->disconnect();
}

void ArmorialVisionUpdater::processDetectionData(const QList<SSL_DetectionFrame> &detectionFrames) {

    // Parse camera data
    QList<std::pair<int,SSL_DetectionBall> > balls = parseCamerasBalls(detectionFrames);
    QHash<int,std::pair<int,SSL_DetectionRobot> > robots = parseCamerasRobots(detectionFrames);

    // Process data
    processBalls(balls);
    processRobots(robots);
}

QList<std::pair<int,SSL_DetectionBall> > ArmorialVisionUpdater::parseCamerasBalls(const QList<SSL_DetectionFrame> &detectionFrames) const {
    QList<std::pair<int,SSL_DetectionBall> > retn;

    // Run on detection frames
    QList<SSL_DetectionFrame>::const_iterator it;
    for(it=detectionFrames.constBegin(); it!=detectionFrames.constEnd(); it++) {
        SSL_DetectionFrame frame = *it;

        // Add to retn list
        for(int i=0; i<frame.balls_size(); i++)
            retn.append(std::make_pair(frame.camera_id(), frame.balls(i)));
    }

    return retn;
}

QHash<int,std::pair<int,SSL_DetectionRobot> > ArmorialVisionUpdater::parseCamerasRobots(const QList<SSL_DetectionFrame> &detectionFrames) const {
    QHash<int,std::pair<int,SSL_DetectionRobot> > retn;

    // Run on detection frames
    QList<SSL_DetectionFrame>::const_iterator it;
    for(it=detectionFrames.constBegin(); it!=detectionFrames.constEnd(); it++) {
        SSL_DetectionFrame frame = *it;

        // Add yellow to retn list
        for(int i=0; i<frame.robots_yellow_size(); i++)
            retn.insertMulti(Colors::YELLOW, std::make_pair(frame.camera_id(), frame.robots_yellow(i)));

        // Add blue to retn list
        for(int i=0; i<frame.robots_blue_size(); i++)
            retn.insertMulti(Colors::BLUE, std::make_pair(frame.camera_id(), frame.robots_blue(i)));
    }

    return retn;
}

void ArmorialVisionUpdater::processBalls(const QList<std::pair<int,SSL_DetectionBall> > &balls) {


    // Add objects to multi object filtering
    QList<std::pair<int,SSL_DetectionBall> >::const_iterator it;
    for(it=balls.constBegin(); it!=balls.constEnd(); it++) {
        const int camId = it->first;
        const SSL_DetectionBall ball = it->second;

        float realX = ball.x()*MM2METER;
        float realY = ball.y()*MM2METER;

        // Field area limit
        if(FieldAreas::hasArea(_fieldLimit, FieldAreas::Q1)==false) {
            if(realX>-0.10 && realY>-0.10)
                continue;
        }
        if(FieldAreas::hasArea(_fieldLimit, FieldAreas::Q2)==false) {
            if(realX<0.10 && realY>-0.10)
                continue;
        }
        if(FieldAreas::hasArea(_fieldLimit, FieldAreas::Q3)==false) {
            if(realX<0.10 && realY<0.10)
                continue;
        }
        if(FieldAreas::hasArea(_fieldLimit, FieldAreas::Q4)==false) {
            if(realX>-0.10 && realY<0.10)
                continue;
        }

        // Confidence
        if(ball.has_confidence()==false)
            continue;

        if(balls.size()>0){ // potencial gigante de erro aqui, verificar depois
            Position *pos_aux = new Position(ball.has_x()&&ball.has_y(), realX, realY, 0.0);;
            Angle *angle_aux = new Angle(true, 0);
            robotsInfo->_ball.update(100, *pos_aux, *angle_aux);
        }
    }
}

void ArmorialVisionUpdater::processRobots(const QHash<int,std::pair<int,SSL_DetectionRobot> > &robots) {

    bool visited[MAX_ROBOTS];
    int qt_blueRobots = robots.size();
    int qt_yellowRobots = detection.robots_yellow_size();
    quint32 camera_id = detection.camera_id();

    if(qt_blueRobots != 0) robotsInfo->_qt_blue=qt_blueRobots;
    if(qt_yellowRobots != 0) robotsInfo->_qt_yellow=qt_yellowRobots;

    robotsInfo->_camera_id=camera_id;

    memset(visited, false, sizeof(visited)); // clear visited robots

    for(int x = 0; x < qt_blueRobots; x++){
        quint32 id = detection.robots_blue(x).robot_id();

        if(!(id < maxRobots)){
            throw std::runtime_error("ID error, check setRobotsInfo");
        }

        Position *pos_aux = new Position(true, detection.robots_blue(x).x(), detection.robots_blue(x).y());
        Angle *angle_aux = new Angle(true, detection.robots_blue(x).orientation());

        robotsInfo->_blueRobots[id].setRobotId(id);
        robotsInfo->_blueRobots[id].update(100, *pos_aux, *angle_aux);

        visited[id] = true; // mark as visited
    }

    for(int x = 0; x < maxRobots && qt_blueRobots != 0; x++){
        if(!visited[x] && !robotsInfo->_blueRobots[x].checkLoss()){
            // se nao tiver sido visitado pelo frame anterior mas ainda estiver rodando loss, da predict
            robotsInfo->_blueRobots[x].predict();
        }
    }

    for(int x = 0; x < qt_yellowRobots; x++){
        quint32 id = detection.robots_yellow(x).robot_id();

        if(!(id < maxRobots)){
            throw std::runtime_error("ID error, check setRobotsInfo");
        }
        Position *pos_aux = new Position(true, detection.robots_yellow(x).x(), detection.robots_yellow(x).y());
        Angle *angle_aux = new Angle(true, detection.robots_yellow(x).orientation());

        robotsInfo->_yellowRobots[id].setRobotId(id);
        robotsInfo->_yellowRobots[id].update(100, *pos_aux, *angle_aux);
    }

    for(int x = 0; x < maxRobots && qt_yellowRobots != 0; x++){
        if(!visited[x] && !robotsInfo->_yellowRobots[x].checkLoss()){
            // se nao tiver sido visitado pelo frame anterior mas ainda estiver rodando loss, da predict
            robotsInfo->_yellowRobots[x].predict();
        }
    }
}

void ArmorialVisionUpdater::processGeometryData(const SSL_GeometryData &geometryData) {

    // Get field data
    const SSL_GeometryFieldSize &field = geometryData.field();

    // Calc centerRadius and areaRadius
    float centerRadius = 0.0;
    float areaRadius = 0.0;
    for(int i=0; i<field.field_arcs_size(); i++) {
        const SSL_FieldCicularArc arc = field.field_arcs(i);
        if(arc.name()=="CenterCircle") // SSL/VSS
            centerRadius = arc.radius() + arc.thickness()/2;
        if(arc.name()=="LeftFieldLeftPenaltyArc") // SSL only
            areaRadius = arc.radius() + arc.thickness()/2;
    }

    // Calc areaWidth and areaStretch
    float areaWidth = 0.0;
    float areaStretch = 0.0;
    for(int i=0; i<field.field_lines_size(); i++) {
        const SSL_FieldLineSegment line = field.field_lines(i);

        // Defense width
        if(line.name()=="RightPenaltyStretch") { // SSL/VSS
            areaWidth = field.field_length()/2 - line.p1().x() - line.thickness()/2;
            areaStretch = 2*fabs(line.p1().y());
            break;
        }
    }

    // Calc goalDepth and areaLength
    float goalDepth = field.goal_depth();
    float areaLength = areaStretch + 2*areaRadius; // SSL/VSS

    // Update to system
    _sensor->setFieldCenter(Position(true, 0.0, 0.0, 0.0));
    _sensor->setFieldTopRightCorner(Position(true, (field.field_length()/2.0)*MM2METER, (field.field_width()/2.0)*MM2METER, 0.0));
    _sensor->setFieldTopLeftCorner(Position(true, (-field.field_length()/2.0)*MM2METER, (field.field_width()/2.0)*MM2METER, 0.0));
    _sensor->setFieldBottomLeftCorner(Position(true, (-field.field_length()/2.0)*MM2METER, (-field.field_width()/2.0)*MM2METER, 0.0));
    _sensor->setFieldBottomRightCorner(Position(true, (field.field_length()/2.0)*MM2METER, (-field.field_width()/2.0)*MM2METER, 0.0));
    _sensor->setRightPenaltyMark(Position(true, (field.field_length()/2.0 - areaRadius)*MM2METER, 0.0, 0.0));
    _sensor->setLeftPenaltyMark(Position(true, (-field.field_length()/2.0 + areaRadius)*MM2METER, 0.0, 0.0));
    _sensor->setLeftGoalPosts(Position(true, (-field.field_length()/2.0)*MM2METER, (-field.egoal_width()/2.0)*MM2METER, 0.0), Position(true, (-field.field_length()/2.0)*MM2METER, (field.goal_width()/2.0)*MM2METER, 0.0));
    _sensor->setRightGoalPosts(Position(true, (field.field_length()/2.0)*MM2METER, (field.goal_width()/2.0)*MM2METER, 0.0), Position(true, (field.field_length()/2.0)*MM2METER, (-field.goal_width()/2.0)*MM2METER, 0.0));
    _sensor->setFieldCenterRadius(centerRadius*MM2METER);
    _sensor->setGoalArea(areaLength*MM2METER, areaWidth*MM2METER, areaRadius*MM2METER);
    _sensor->setGoalDepth(goalDepth*MM2METER);

    if(_debugGeometry) {
        printf("[Geometry Data]\n");
        printf("Field Dimensions:\n");
        printf("  -field_length=%d (mm)\n",field.field_length());
        printf("  -field_width=%d (mm)\n",field.field_width());
        printf("  -boundary_width=%d (mm)\n",field.boundary_width());
        printf("  -goal_width=%d (mm)\n",field.goal_width());
        printf("  -goal_depth=%d (mm)\n",field.goal_depth());
    }
}

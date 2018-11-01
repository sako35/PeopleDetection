// -*- C++ -*-
/*!
 * @file  PeopleDetection.cpp
 * @brief It is an RTC that performs people detection using Kinect v2.
 * @date $Date$
 *
 * $Id$
 */

#include "PeopleDetection.h"

//***** add *****************************************************
//---Kinect�֌W
// Kinect
CComPtr<IKinectSensor> kinect = nullptr;

// Kinect -body-
CComPtr<IBodyFrameReader> bodyFR = nullptr;
CComPtr<IBodyFrameSource> bodyFS = nullptr;
IBody* bodies[BODY_COUNT];

// Kinect -color-
CComPtr<IColorFrameReader> colorFR = nullptr;
CComPtr<IColorFrameSource> colorFS = nullptr;
CComPtr<IFrameDescription> colorFD = nullptr;
int colorW;
int colorH;
unsigned int colorBPP;
ColorImageFormat colorF = ColorImageFormat::ColorImageFormat_Bgra;
std::vector<BYTE> colorB;

//***** END add ***********************************************


// Module specification
// <rtc-template block="module_spec">
static const char* peopledetection_spec[] =
  {
    "implementation_id", "PeopleDetection",
    "type_name",         "PeopleDetection",
    "description",       "It is an RTC that performs people detection using Kinect v2.",
    "version",           "1.0.0",
    "vendor",            "sako",
    "category",          "Category",
    "activity_type",     "SPORADIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.person_or_people", "people",
    "conf.default.yardstick", "xz",
    "conf.default.joint_for_getting_position", "1",

    // Widget
    "conf.__widget__.person_or_people", "radio",
    "conf.__widget__.yardstick", "radio",
    "conf.__widget__.joint_for_getting_position", "text",
    // Constraints
    "conf.__constraints__.person_or_people", "(person, people)",
    "conf.__constraints__.yardstick", "(x,xz)",

    "conf.__type__.person_or_people", "string",
    "conf.__type__.yardstick", "string",
    "conf.__type__.joint_for_getting_position", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
PeopleDetection::PeopleDetection(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_num_peopleOut("num_people", m_num_people),
    m_positionsOut("positions", m_positions)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
PeopleDetection::~PeopleDetection()
{
}


//***** add *********************************************************************
//+++++ ret�@���ł������ǂ����m�F���邽�߂̊֐� +++++
// �ł��Ȃ������ꍇ�ARTC::RTC_ERROR��Ԃ��c�͂��H
#define ERROR_CHECK(ret) if((ret) != S_OK){std::stringstream ss; ss << "failed " #ret " " << std::hex << ret << std::endl; throw RTC::RTC_ERROR;}

//+++++ �֐߁E�[�_�Ɋۂ�`�悷�邽�߂̊֐� +++++
void drawEllipse(cv::Mat& Image, const Joint& joint, int r, const cv::Scalar& color)
{
	// ���W�ϊ��p�̉������擾
	CComPtr<ICoordinateMapper> mapper;
	ERROR_CHECK(kinect->get_CoordinateMapper(&mapper));
	// �J�������W�̃|�C���g���J���[���W�ɕϊ�������ɓ���p�̕ϐ��̊m��
	ColorSpacePoint point;
	// �J�������W�̃|�C���g���J���[���W�ɕϊ�
	mapper->MapCameraPointToColorSpace(joint.Position, &point);
	// �֐߁E�[�_�����Ɋۂ�`��
	cv::circle(Image, cv::Point(point.X, point.Y), r, color, -1);
}

float Distance_xz(float x, float z)
{
	float distance = (x*x + z*z)*(1 / 2);
	return distance;
}

//***** END add *****************************************************************


RTC::ReturnCode_t PeopleDetection::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("num_people", m_num_peopleOut);
  addOutPort("positions", m_positionsOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("person_or_people", m_person_or_people, "people");
  bindParameter("yardstick", m_yardstick, "xz");
  bindParameter("joint_for_getting_position", m_joint_for_getting_position, "1");
  // </rtc-template>
  
  //***** add **************************************************************
  //---Start Kinect
  // �L�l�N�g���擾
  ERROR_CHECK(GetDefaultKinectSensor(&kinect));
  // �L�l�N�g���J��
  // �����́��̓A���[���Z�q�Ƃ����B�ڂ����̓A���[���Z�q�Ƃ��|�C���^�Ƃ��ŃO�O��B�Ӗ��I�ɂ́wkinect�̒���Open���Ă̂�����x�Ƃ��������B
  ERROR_CHECK(kinect->Open());
  // �����܂łł�����wKinect open�x�ƕ\��
  std::cout << "Kinect open" << std::endl;

  //---Kinect�ŃJ���[�摜���Ƃ鏀��
  // �J���[�t���[���\�[�X���擾
  ERROR_CHECK(kinect->get_ColorFrameSource(&colorFS));
  // �J���[�t���[�����[�_�[���J��
  ERROR_CHECK(colorFS->OpenReader(&colorFR));
  // �t���[���̏ڍׂ��擾�H�쐬�H
  ERROR_CHECK(colorFS->CreateFrameDescription(ColorImageFormat::ColorImageFormat_Bgra, &colorFD));
  // �J���[�摜�̉������擾
  ERROR_CHECK(colorFD->get_Width(&colorW));
  // �J���[�摜�̍������擾
  ERROR_CHECK(colorFD->get_Height(&colorH));
  // �J���[�摜��BPP���擾
  ERROR_CHECK(colorFD->get_BytesPerPixel(&colorBPP));
  // �J���[�摜�̃o�b�t�@���쐬
  colorB.resize(colorW * colorH * colorBPP);

  //---Kinect��body���Ƃ鏀��
  // �{�f�B�t���[���\�[�X���擾
  ERROR_CHECK(kinect->get_BodyFrameSource(&bodyFS));
  // �{�f�B�t���[�����[�_�[���J��
  ERROR_CHECK(bodyFS->OpenReader(&bodyFR));
  // �{�f�B�z���������
  for (auto& body : bodies)
  {
	  body = nullptr;
  }
  //***** END add **********************************************************


  return RTC::RTC_OK;
}


RTC::ReturnCode_t PeopleDetection::onFinalize()
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t PeopleDetection::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PeopleDetection::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t PeopleDetection::onActivated(RTC::UniqueId ec_id)
{
	// �o�̓f�[�^�̏���
	m_num_people.data = 0;

	// �{�f�B�z���������
	for (auto& body : bodies)
	{
		body = nullptr;
	}
  return RTC::RTC_OK;
}


RTC::ReturnCode_t PeopleDetection::onDeactivated(RTC::UniqueId ec_id)
{

	cv::destroyAllWindows();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t PeopleDetection::onExecute(RTC::UniqueId ec_id)
{
	//---�J���[�摜�̏���
	// �J���[�t���[�����擾����
	CComPtr<IColorFrame> colorFrame;
	ERROR_CHECK(colorFR->AcquireLatestFrame(&colorFrame));
	ERROR_CHECK(colorFrame->CopyConvertedFrameDataToArray(colorB.size(), &colorB[0], colorF));
	// �J���[�摜���쐬
	cv::Mat colorImage(colorH, colorW, CV_8UC4, &colorB[0]);

	//---�{�f�B�̏���
	CComPtr<IBodyFrame> bodyF;
	// �V�����{�f�B�t���[�����󂯎��
	ERROR_CHECK(bodyFR->AcquireLatestFrame(&bodyF));

	// ���o�l���̃��Z�b�g
	m_num_people.data = 0;


	//---��l���o���[�h�̎�
	if (m_person_or_people == "person")
	{
		//***** "hitori"���[�h�ɂ����Ȃ����� *****************************
		//--��r���邽�߂̕ϐ�
		// Kinect����̍ł��߂��l�ւ̋���
		float closest_distance = 20000000;
		// ��r���鋗��
		float distance = 0;
		// ��l�ł��W���C���g���擾�����ʂ܂ōs�������ǂ����̊m�F�ϐ�
		bool hito_iruka = false;
		// ��ԋ߂��l�̊֐߁E�[�_�z�����邽�߂̔z��
		Joint closest_joints[JointType::JointType_Count];
		// ���o�l���͈�l
		m_num_people.data = 1;
		//***** END "hitori"���[�h�ɂ����Ȃ����� *****************************

		for (auto& body : bodies)
		{
			if (body != nullptr){
				body->Release();
				body = nullptr;
			}
		}
		ERROR_CHECK(bodyF->GetAndRefreshBodyData(6, &bodies[0]));
		m_positions.data.length(3 * 6);
		const int R = 10;
		for (auto body : bodies)
		{
			if (body == nullptr)
			{
				continue;
			}
			BOOLEAN isTracked = false;
			ERROR_CHECK(body->get_IsTracked(&isTracked));
			if (!isTracked)
			{
				continue;
			}
			//***** "hitori���[�h�ɂ����Ȃ�����****************************************
			// �l���͂P�l�Ŋm��Ȃ̂ł���͂���Ȃ�
			// m_ninzu.data++;
			// �P�l�ł��g���b�L���O����Ă��炱�ꂪtrue�ɂȂ�
			hito_iruka = true;
			//***** END "hitori���[�h�ɂ����Ȃ�����****************************************

			Joint joints[JointType::JointType_Count];
			body->GetJoints(JointType::JointType_Count, joints);

			//***** "hitori���[�h�ɂ����Ȃ�����****************************************
			// ���ݏ������Ă���{�f�B�̋������擾����
			// z���̋����݂̂ő��肷��o�[�W����
			if (m_yardstick == "z")
			{
				distance = joints[m_joint_for_getting_position].Position.Z;
			}
			// x,y,z �̋����ő��肷��o�[�W�����iy����Ȃ��ˁH�j
			else if (m_yardstick == "xz")
			{
				distance = Distance_xz(joints[m_joint_for_getting_position].Position.X, joints[m_joint_for_getting_position].Position.Z);
			}

			// �������O�̂��̂��Z���A0�łȂ��ꍇ�A�X�V����
			if (closest_distance >= distance && distance != 0)
			{
				// �����̍X�V
				closest_distance = distance;
				// �֐߁E�[�_�z��̍X�V
				memcpy(closest_joints, joints, sizeof(joints));
			}
			//***** END "hitori���[�h�ɂ����Ȃ�����****************************************
		}

		// �P�l�ł����o���Ă�����A�W���C���g��`�悷��
		if (hito_iruka)
		{
			for (auto joint : closest_joints)
			{
				drawEllipse(colorImage, joint, R, cv::Scalar(255, 0, 0));
			}
			// ����p�f�[�^�̍쐬
			m_positions.data[0] = closest_joints[m_joint_for_getting_position].Position.X;
			m_positions.data[1] = closest_joints[m_joint_for_getting_position].Position.Y;
			m_positions.data[2] = closest_joints[m_joint_for_getting_position].Position.Z;
		}

		// �摜�̕\��
		cv::resize(colorImage, colorImage, cv::Size(), 0.5, 0.5);
		cv::imshow("Image", colorImage);
		cv::waitKey(1);

		m_num_peopleOut.write();
		m_positionsOut.write();

	}
	//---�S�����o���[�h�̎�
	else if (m_person_or_people == "people")
	{
		// �ߋ���bodies�z��������[�X
		for (auto& body : bodies)
		{
			// body���k���|����Ȃ�������
			if (body != nullptr){
				// �{�f�B�������[�X���āc
				body->Release();
				// �{�f�B���k���|�ɂ���
				body = nullptr;
			}
		}

		// �{�f�B�f�[�^���擾����
		ERROR_CHECK(bodyF->GetAndRefreshBodyData(6, &bodies[0]));
		// �ʒu�o�͗p�z��̑傫�������߂�
		// (��l������)3��(x, y, z) * 6�l
		m_positions.data.length(3 * 6);

		//--�֐߁E�[�_�ʒu�̎擾
		// �֐߁E�[�_�ʒu�ɕ`���ۂ̑傫��
		const int R = 10;

		// bodies�z��ɓ���Ă���body���ɑ΂��ď������s��
		for (auto body : bodies)
		{
			// �����{�f�B���k���|��������A���̃{�f�B�Ɋւ��Ă͈ȍ~�̏������΂�
			if (body == nullptr)
			{
				continue;
			}

			// �{�f�B���g���b�L���O�ł��Ă邩�m���߂邽�߂̕ϐ�
			BOOLEAN isTracked = false;
			//--�{�f�B���g���b�L���O����Ă��邩���擾����
			ERROR_CHECK(body->get_IsTracked(&isTracked));
			// �g���b�L���O����Ă��Ȃ�������A���̃{�f�B�Ɋւ��Ă͈ȍ~�̏������΂�
			if (!isTracked)
			{
				continue;
			}
			// �g���b�L���O����Ă���l�����J�E���g����
			m_num_people.data++;
			// �֐߁E�[�_�ʒu���擾����p�̔z����m��
			Joint joints[JointType::JointType_Count];
			// �֐߁E�[�_�ʒu���擾
			body->GetJoints(JointType::JointType_Count, joints);
			// �e�֐߁E�[�_�ʒu�Ɋւ��ĕ`�揈�����s��
			for (auto joint : joints)
			{
				// �֐߁E�[�_�ʒu���w�g���b�L���O��ԁx��������w�H�x�ۂ�`��
				if (joint.TrackingState == TrackingState::TrackingState_Tracked)
				{
					drawEllipse(colorImage, joint, R, cv::Scalar(255, 0, 0));
				}
				// �֐߁E�[�_�ʒu���w�g���b�L���O��ԁx��������w���F�H�x�ۂ�`��
				if (joint.TrackingState == TrackingState::TrackingState_Inferred)
				{
					drawEllipse(colorImage, joint, R, cv::Scalar(255, 255, 0));
				}
			}

			// ���o���ꂽ�l�̊֐߁E�[�_�ʒu��z��ɕۑ�
			m_positions.data[0 + (m_num_people.data - 1) * 3] = joints[m_joint_for_getting_position].Position.X;
			m_positions.data[1 + (m_num_people.data - 1) * 3] = joints[m_joint_for_getting_position].Position.Y;
			m_positions.data[2 + (m_num_people.data - 1) * 3] = joints[m_joint_for_getting_position].Position.Z;
		}

		// �摜�̕\��
		cv::resize(colorImage, colorImage, cv::Size(), 0.5, 0.5);
		cv::imshow("Image", colorImage);
		cv::waitKey(1);

		m_num_peopleOut.write();
		m_positionsOut.write();
	}
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t PeopleDetection::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PeopleDetection::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PeopleDetection::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PeopleDetection::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t PeopleDetection::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void PeopleDetectionInit(RTC::Manager* manager)
  {
    coil::Properties profile(peopledetection_spec);
    manager->registerFactory(profile,
                             RTC::Create<PeopleDetection>,
                             RTC::Delete<PeopleDetection>);
  }
  
};



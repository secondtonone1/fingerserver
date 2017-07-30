
AUTOMAKE_OPTIONS = foreign

SUBDIRS = PlatformLib NetworkLib CommonLib

#g++ tmptest.cpp -I./jsoncpp/include -L./jsoncpp/libs/linux-gcc-4.4.7/ -ljson_linux-gcc-4.4.7_libmt

AM_CPPFLAGS = -Wno-deprecated -I./ -I./PlatformLib -I./NetworkLib -I./CommonLib -I./MysqlLib	\
 	-I./FireConfirm	-I./jsoncpp/include -I./hiredis -I./Log4cxx/include  \
	-D JSON_HAS_INT6464 -D_LYNX_BYTE_ORDER_NOT_CONVERT -D_LYNX_DEBUG -D_LYNX_HAS_LIBICONV -D __linux__


AM_LDFLAGS = -L./ -L./PlatformLib -L./NetworkLib -L./CommonLib -L./Linux 

LDADD = -lcommon -lplatform -lnetwork -lpthread -ljson_linux-gcc-4.4.7_libmt -lmysql -lhiredis   -llog4cxx


install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/tekfi_server_$(RELEASE)/
	$(INSTALL_DATA) -m 700 gameserver $(DESTDIR)$(prefix)/tekfi_server_$(RELEASE)/

noinst_PROGRAMS = gameserver
gameserver_SOURCES = \
	Logcxx.cpp \
	FireConfirm/Robot.cpp \
	FireConfirm/RankGame.cpp \
	PKSystem/pvpthread.cpp	\
	PKSystem/PVPSystemWC.cpp \
	FireConfirm/GlobalValue.cpp	\
	FireConfirm/InlineActivity.cpp \
	FireConfirm/CourageChallenge.cpp \
	FireConfirm/Code.cpp \
	ConsortSystem/ConsortSystem.cpp \
	EmailMsgHandler.cpp\
	AchieveMsgHandler.cpp\
	AchieveManager.cpp\
	ServerData.cpp\
	CharactorMsgHandler.cpp\
	CharactorManager.cpp\
    ConsortMsgHandler.cpp\
	FashionMsgHandler.cpp\
	EmailManager.cpp	\
	PlayerAttrData.cpp	\
	RankSystem.cpp	\
	FriendMsgHandler.cpp	\
	FriendBlackManager.cpp	\
	MemDataSystem.cpp	\
	ChatMsgHandler.cpp\
	ChatManager.cpp\
	ServantMsgHandler.cpp\
	ServantManager.cpp\
	Guild.cpp \
	AllItemManager.cpp\
	Ranking/RankingPersonal.cpp\
	Ranking/Ranking.cpp\
	GameRandom.cpp \
	JewelryMsgHandler.cpp\
	JewelryManager.cpp\
	Buy/Buy.cpp\
	Buy/Trading.cpp \
	DebugMsgHandler.cpp \
	IndulgeSystem.cpp \
	IndulgeWorker.cpp \
	InterfaceSystem.cpp \
	InterfaceWorker.cpp \
	RhymeMsgHandler.cpp \
	HeroEquipManager.cpp \
	HeroEquipMsgHandler.cpp	\
	RhymeManager.cpp \
        RedisManager.cpp \
	LogicSystem.cpp \
	PersistManager.cpp \
	ItemMsgHandler.cpp \
	MaterialManager.cpp \
	ItemManager.cpp	\
	EquipManager.cpp \
	StageManager.cpp \
	FashionManager.cpp \
	DetailInfoManager.cpp \
	GMMessageHandler.cpp \
	PVPMsgHandler.cpp \
	GMManager.cpp \
	TimeManager.cpp \
	BuffManager.cpp \
	PlayerInfoManager.cpp \
	ConsortManager.cpp \
	Player.cpp \
	PersistSystem.cpp \
	PersistWorkerExt.cpp\
	PersistWorker.cpp \
	HoarStoneManager.cpp \
	HoarStoneMsgHandler.cpp \
	CommonMsgHandler.cpp \
	LoginMsgHandler.cpp \
	DetailInfoMsgHandler.cpp \
	SkillMsgHandler.cpp \
	FireConfirm/Shop.cpp \
	FireConfirm/ClimbTower.cpp \
	FireConfirm/NewException.cpp \
	FireConfirm/BaseChang.cpp \
	FireConfirm/LocalData.cpp \
	FireConfirm/StageCalc.cpp \
	FireConfirm/BattleCalc.cpp \
	FireConfirm/Box.cpp \
	FireConfirm/Gift.cpp \
	FireConfirm/FireConfirmManager.cpp \
	FireConfirm/Character.cpp \
	FireConfirm/Drop.cpp \
	FireConfirm/PVPSystem.cpp \
	FireConfirm/PVPWorker.cpp \
	FireConfirm/Scene.cpp \
	ClientConnection.cpp \
	ListenerWorker.cpp \
	MainThread.cpp \
	NetworkWorker.cpp \
	NetworkSystem.cpp \
	SkillManager.cpp \
	LoggerWorker.cpp \
	LoggerSystem.cpp \
	PassportWorker.cpp \
	PassportSystem.cpp \
	GemManager.cpp\
	GameServer.cpp\
	GemMsgHandler.cpp


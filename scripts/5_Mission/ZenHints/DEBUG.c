/*
class ZenHintsMockQueueSettings
{
	static const bool ENABLE_MOCK_QUEUE_SCREEN = true;
	static const int MOCK_QUEUE_POSITION = 999;
}

class ZenHintsMockLoginQueueScreen extends LoginQueueBase
{
	protected bool m_ZenMockInitialized;

	override Widget Init()
	{
		Widget rootWidget = super.Init();

		m_ZenMockInitialized = true;

		Print("[ZenHintsDebug] MOCK_QUEUE: Init complete.");

		return rootWidget;
	}

	override void Show()
	{
		if (!m_ZenMockInitialized)
		{
			Init();
		}

		super.Show();

		SetPosition(ZenHintsMockQueueSettings.MOCK_QUEUE_POSITION);

		ZenApplyQueueHintContext();

		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ZenApplyQueueHintContext, ZenHintsQueueDebugSettings.REAPPLY_DELAY_1_MS, false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ZenApplyQueueHintContext, ZenHintsQueueDebugSettings.REAPPLY_DELAY_2_MS, false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ZenApplyQueueHintContext, ZenHintsQueueDebugSettings.REAPPLY_DELAY_3_MS, false);

		Print("[ZenHintsDebug] MOCK_QUEUE: Show complete.");
	}

	override void Hide()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(ZenApplyQueueHintContext);

		super.Hide();

		Print("[ZenHintsDebug] MOCK_QUEUE: Hide complete.");
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_btnLeave)
		{
			DestroyMock();
			return true;
		}

		return super.OnClick(w, x, y, button);
	}

	void DestroyMock()
	{
		Hide();

		if (layoutRoot)
		{
			layoutRoot.Unlink();
			layoutRoot = NULL;
		}

		m_HintPanel = NULL;
		m_ZenMockInitialized = false;

		Print("[ZenHintsDebug] MOCK_QUEUE: destroyed.");
	}

	bool ZenMockIsVisible()
	{
		if (!layoutRoot)
		{
			return false;
		}

		return layoutRoot.IsVisible();
	}

	void Toggle()
	{
		if (ZenMockIsVisible())
		{
			DestroyMock();
			return;
		}

		Show();
	}

	override protected void ZenApplyQueueHintContext()
	{
		ZenHintsQueueIconHelper.ApplyQueueHintContext(layoutRoot, m_HintPanel, "MOCK_QUEUE");
	}
}

modded class MissionGameplay
{
	protected ref ZenHintsMockLoginQueueScreen m_ZenHintsMockLoginQueueScreen;
	protected bool m_ZenHintsMockQueueKeyWasDown;

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		if (!ZenHintsMockQueueSettings.ENABLE_MOCK_QUEUE_SCREEN)
		{
			return;
		}

		bool keyIsDown = GetGame().GetInput().KeyState(KeyCode.KC_L);

		if (keyIsDown && !m_ZenHintsMockQueueKeyWasDown)
		{
			ToggleZenHintsMockLoginQueue();
		}

		m_ZenHintsMockQueueKeyWasDown = keyIsDown;

		if (m_ZenHintsMockLoginQueueScreen)
		{
			if (m_ZenHintsMockLoginQueueScreen.ZenMockIsVisible())
			{
				m_ZenHintsMockLoginQueueScreen.Update(timeslice);
			}
		}
	}

	protected void ToggleZenHintsMockLoginQueue()
	{
		if (!m_ZenHintsMockLoginQueueScreen)
		{
			m_ZenHintsMockLoginQueueScreen = new ZenHintsMockLoginQueueScreen();
		}

		m_ZenHintsMockLoginQueueScreen.Toggle();

		if (!m_ZenHintsMockLoginQueueScreen.ZenMockIsVisible())
		{
			delete m_ZenHintsMockLoginQueueScreen;
			m_ZenHintsMockLoginQueueScreen = NULL;
		}
	}
}
*/
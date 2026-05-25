/*
class ZenHintsQueueDebugSettings
{
	static const bool ENABLE_DEBUG_LOGS = true;
	static const bool ENABLE_REAL_QUEUE_PATCH = true;

	static const float HINT_ICON_BASE_SIZE = 48.0;
	static const float HINT_ICON_BASE_SCREEN_HEIGHT = 1080.0;
	static const float HINT_ICON_MIN_SIZE = 48.0;
	static const float HINT_ICON_MAX_SIZE = 72.0;

	static const float HINT_ICON_POS_X = 0.0;
	static const float HINT_ICON_POS_Y = 0.17;

	static const int REAPPLY_DELAY_1_MS = 100;
	static const int REAPPLY_DELAY_2_MS = 500;
	static const int REAPPLY_DELAY_3_MS = 1000;
}

class ZenHintsQueueIconHelper
{
	static void ZDebugPrint(string message)
	{
		if (!ZenHintsQueueDebugSettings.ENABLE_DEBUG_LOGS)
		{
			return;
		}

		Print("[ZenHintsDebug] " + message);
	}

	static float GetScaledHintIconSize()
	{
		int screenWidth;
		int screenHeight;

		GetScreenSize(screenWidth, screenHeight);

		float iconSize = ZenHintsQueueDebugSettings.HINT_ICON_BASE_SIZE * (screenHeight / ZenHintsQueueDebugSettings.HINT_ICON_BASE_SCREEN_HEIGHT);

		if (iconSize < ZenHintsQueueDebugSettings.HINT_ICON_MIN_SIZE)
		{
			iconSize = ZenHintsQueueDebugSettings.HINT_ICON_MIN_SIZE;
		}

		if (iconSize > ZenHintsQueueDebugSettings.HINT_ICON_MAX_SIZE)
		{
			iconSize = ZenHintsQueueDebugSettings.HINT_ICON_MAX_SIZE;
		}

		return iconSize;
	}

	static void ApplyIconLayout(ImageWidget iconWidget)
	{
		if (!iconWidget)
		{
			return;
		}

		float iconSize = GetScaledHintIconSize();

		iconWidget.SetPos(ZenHintsQueueDebugSettings.HINT_ICON_POS_X, ZenHintsQueueDebugSettings.HINT_ICON_POS_Y);
		iconWidget.SetSize(iconSize, iconSize);
		iconWidget.SetFlags(WidgetFlags.STRETCH, true);
		iconWidget.Show(true);
	}

	static ImageWidget GetRootHintIcon(Widget layoutRoot)
	{
		if (!layoutRoot)
		{
			return NULL;
		}

		return ImageWidget.Cast(layoutRoot.FindAnyWidget("hintIcon"));
	}

	static void DebugWidget(string label, Widget widget)
	{
		if (!ZenHintsQueueDebugSettings.ENABLE_DEBUG_LOGS)
		{
			return;
		}

		if (!widget)
		{
			Print("[ZenHintsDebug] " + label + " is null.");
			return;
		}

		float posX;
		float posY;
		float sizeX;
		float sizeY;

		widget.GetPos(posX, posY);
		widget.GetSize(sizeX, sizeY);

		Print("[ZenHintsDebug] " + label + " pos=" + posX + " " + posY + " size=" + sizeX + " " + sizeY + " visible=" + widget.IsVisible());
	}

	static void ApplyQueueHintContext(Widget layoutRoot, UiHintPanel hintPanel, string debugLabel)
	{
		if (!layoutRoot)
		{
			Print("[ZenHintsDebug] " + debugLabel + ": layoutRoot is null.");
			return;
		}

		if (!hintPanel)
		{
			Print("[ZenHintsDebug] " + debugLabel + ": hintPanel is null.");
			return;
		}

		ImageWidget rootHintIcon = GetRootHintIcon(layoutRoot);

		if (!rootHintIcon)
		{
			Print("[ZenHintsDebug] " + debugLabel + ": root hintIcon not found.");
			return;
		}

		DebugWidget(debugLabel + " root hintIcon before", rootHintIcon);

		ApplyIconLayout(rootHintIcon);
		hintPanel.SetZenLoadingScreenContext(rootHintIcon, layoutRoot);
		ApplyIconLayout(rootHintIcon);

		DebugWidget(debugLabel + " root hintIcon after", rootHintIcon);

		ZDebugPrint(debugLabel + ": queue hint context applied.");
	}
}

modded class UiHintPanel
{
	protected ImageWidget m_ZenHintIcon;
	protected Widget m_ZenHintRootWidget;
	protected bool m_ZenIsLoadingScreenContext;

	override void LoadContentList()
	{
		super.LoadContentList();

		if (GetZenHintsConfig().SyncVersion != ZenHintsConfig.INIT_VERSION)
		{
			GetZenHintsConfig().CheckSingleEntry();

			if (GetZenHintsConfig().ReplaceVanillaInsteadOfMerge)
			{
				m_ContentList.Clear();
			}

			foreach (HintPage hintPage : GetZenHintsConfig().Hints)
			{
				m_ContentList.Insert(hintPage);
			}
		}
	}

	void SetZenLoadingScreenContext(ImageWidget iconWidget, Widget rootWidget)
	{
		if (!iconWidget)
		{
			return;
		}

		if (!rootWidget)
		{
			return;
		}

		m_ZenHintIcon = iconWidget;
		m_ZenHintRootWidget = rootWidget;
		m_ZenIsLoadingScreenContext = true;

		ApplyZenHintIconLayout();
		HideVanillaHintImageForLoadingScreen();
		ZenSetHintImage();
	}

	void ClearZenLoadingScreenContext()
	{
		m_ZenHintIcon = null;
		m_ZenHintRootWidget = null;
		m_ZenIsLoadingScreenContext = false;
	}

	protected bool IsZenLoadingScreenContext()
	{
		if (!m_ZenIsLoadingScreenContext)
		{
			return false;
		}

		if (!m_ZenHintIcon)
		{
			return false;
		}

		return true;
	}

	protected void HideVanillaHintImageForLoadingScreen()
	{
		if (!m_ZenHintRootWidget)
		{
			return;
		}

		ImageWidget vanillaHintImage = ImageWidget.Cast(m_ZenHintRootWidget.FindAnyWidget("HintImage"));

		if (vanillaHintImage && vanillaHintImage != m_ZenHintIcon)
		{
			vanillaHintImage.Show(false);
		}
	}

	protected void ApplyZenHintIconLayout()
	{
		if (!m_ZenHintIcon)
		{
			return;
		}

		ZenHintsQueueIconHelper.ApplyIconLayout(m_ZenHintIcon);
	}

	protected void ZenSetHintImage()
	{
		HintPage currentHint = m_ContentList.Get(m_PageIndex);

		if (!currentHint)
		{
			return;
		}

		if (!IsZenLoadingScreenContext())
		{
			return;
		}

		ApplyZenHintIconLayout();
		HideVanillaHintImageForLoadingScreen();

		if (!currentHint.IsZenHint())
		{
			ResetZenImageIcon();
			return;
		}

		string imagePath = currentHint.GetImagePath();

		if (imagePath != string.Empty)
		{
			m_ZenHintIcon.LoadImageFile(0, imagePath);
			ApplyZenHintIconLayout();
			return;
		}

		ResetZenImageIcon();
	}

	override protected void SetHintImage()
	{
		if (IsZenLoadingScreenContext())
		{
			ZenSetHintImage();
			return;
		}

		super.SetHintImage();
	}

	void ResetZenImageIcon()
	{
		if (!m_ZenHintIcon)
		{
			return;
		}

		m_ZenHintIcon.LoadImageFile(0, "set:dayz_gui image:loading_screen_bulb");
		ApplyZenHintIconLayout();
	}
}

modded class UiHintPanelLoading
{
	override protected void SetHintImage()
	{
		if (IsZenLoadingScreenContext())
		{
			ZenSetHintImage();
			return;
		}

		super.SetHintImage();
	}
}

modded class LoadingScreen
{
	override void Show()
	{
		super.Show();

		ImageWidget iconWidget = ImageWidget.Cast(m_WidgetRoot.FindAnyWidget("hintIcon"));

		if (m_HintPanel)
		{
			m_HintPanel.SetZenLoadingScreenContext(iconWidget, m_WidgetRoot);
		}
	}
}

modded class LoginTimeBase
{
	override void Show()
	{
		super.Show();

		ImageWidget iconWidget = ImageWidget.Cast(layoutRoot.FindAnyWidget("hintIcon"));

		if (m_HintPanel)
		{
			m_HintPanel.SetZenLoadingScreenContext(iconWidget, layoutRoot);
		}
	}
}

modded class LoginQueueBase
{
	override void Show()
	{
		super.Show();

		if (!ZenHintsQueueDebugSettings.ENABLE_REAL_QUEUE_PATCH)
		{
			return;
		}

		ZenApplyQueueHintContext();
	}

	protected void ZenApplyQueueHintContext()
	{
		ZenHintsQueueIconHelper.ApplyQueueHintContext(layoutRoot, m_HintPanel, "QUEUE");
	}
}
*/
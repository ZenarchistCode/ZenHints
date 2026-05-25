class ZenHintsLoadingIconHelper
{
	static float GetScaledHintIconSize()
	{
		int screenWidth;
		int screenHeight;

		GetScreenSize(screenWidth, screenHeight);

		float iconSize = 48.0 * (screenHeight / 1080.0);

		if (iconSize < 48.0)
		{
			iconSize = 48.0;
		}

		if (iconSize > 72.0)
		{
			iconSize = 72.0;
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

		iconWidget.SetPos(0.0, 0.17);
		iconWidget.SetSize(iconSize, iconSize);
		iconWidget.SetFlags(WidgetFlags.STRETCH, true);
		iconWidget.Show(true);
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

		ZenHintsLoadingIconHelper.ApplyIconLayout(m_ZenHintIcon);
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
	protected bool m_ZenHintContextApplied;

	override Widget Init()
	{
		Widget rootWidget = super.Init();

		ZenApplyQueueHintContext();

		return rootWidget;
	}

	override void SetPosition(int position)
	{
		super.SetPosition(position);

		if (!m_ZenHintContextApplied)
		{
			ZenApplyQueueHintContext();
		}
	}

	protected void ZenApplyQueueHintContext()
	{
		if (!layoutRoot)
		{
			Print("[ZenHints] LoginQueueBase layoutRoot is null.");
			return;
		}

		if (!m_HintPanel)
		{
			Print("[ZenHints] LoginQueueBase m_HintPanel is null.");
			return;
		}

		ImageWidget iconWidget = ImageWidget.Cast(layoutRoot.FindAnyWidget("hintIcon"));

		if (!iconWidget)
		{
			Print("[ZenHints] LoginQueueBase root hintIcon not found.");
			return;
		}

		ZenHintsLoadingIconHelper.ApplyIconLayout(iconWidget);

		m_HintPanel.SetZenLoadingScreenContext(iconWidget, layoutRoot);

		ZenHintsLoadingIconHelper.ApplyIconLayout(iconWidget);

		m_ZenHintContextApplied = true;
	}
}
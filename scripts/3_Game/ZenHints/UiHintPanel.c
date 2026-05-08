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
			
			//Print("[ZenHints] Loaded custom hints. Total hints in array: " + m_ContentList.Count() + "/" + GetZenHintsConfig().Hints.Count());
		}
	}

	void SetZenLoadingScreenContext(ImageWidget iconWidget, Widget rootWidget)
	{
		m_ZenHintIcon = iconWidget;
		m_ZenHintRootWidget = rootWidget;
		m_ZenIsLoadingScreenContext = true;

		HideVanillaHintImageForLoadingScreen();
		SetHintImage();
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
		if (!vanillaHintImage)
		{
			return;
		}

		vanillaHintImage.Show(false);
	}

	override protected void SetHintImage()
	{
		HintPage currentHint = m_ContentList.Get(m_PageIndex);
		if (!currentHint)
		{
			return;
		}

		if (!IsZenLoadingScreenContext())
		{
			super.SetHintImage();
			return;
		}

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
			m_ZenHintIcon.SetFlags(WidgetFlags.STRETCH, true);
			return;
		}

		ResetZenImageIcon();
	}

	void ResetZenImageIcon()
	{
		if (!m_ZenHintIcon)
		{
			return;
		}

		m_ZenHintIcon.LoadImageFile(0, "set:dayz_gui image:loading_screen_bulb");
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

		ImageWidget iconWidget = ImageWidget.Cast(layoutRoot.FindAnyWidget("hintIcon"));

		if (m_HintPanel)
		{
			m_HintPanel.SetZenLoadingScreenContext(iconWidget, layoutRoot);
		}
	}
}

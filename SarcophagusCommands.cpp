#include "pch.h"
#include "SarcophagusCommands.h"

#if __has_include("EditCredentialCommand.g.cpp")
#include "EditCredentialCommand.g.cpp"
#endif
#if __has_include("AddCredentialCommand.g.cpp")
#include "AddCredentialCommand.g.cpp"
#endif
#if __has_include("RemoveCredentialCommand.g.cpp")
#include "RemoveCredentialCommand.g.cpp"
#endif
#if __has_include("CopyCredentialCommand.g.cpp")
#include "CopyCredentialCommand.g.cpp"
#endif

namespace winrt::Sarcophagus::implementation
{
	void AddCredentialCommand::Execute(IInspectable const&)
	{
		Sarcophagus::MainVM mainVM = Sarcophagus::MainVM::GetInstance();
		mainVM.Credentials().Append(Sarcophagus::Credential(L"New credential", L""));
	}

	void RemoveCredentialCommand::Execute(IInspectable const& parameter)
	{
		Sarcophagus::MainVM mainVM = Sarcophagus::MainVM::GetInstance();

		if (auto credential = parameter.try_as<Sarcophagus::Credential>())
		{
			uint32_t index;
			if (mainVM.Credentials().IndexOf(credential, index))
			{
				mainVM.Credentials().RemoveAt(index);
			}
		}
	}

	void EditCredentialCommand::Execute(IInspectable const& parameter)
	{
		Sarcophagus::MainVM mainVM = Sarcophagus::MainVM::GetInstance();

		if (auto credential = parameter.try_as<Sarcophagus::Credential>())
		{
			mainVM.CredentialTemplate(credential);
			mainVM.PageId(Sarcophagus::PageId::EditCredential);
		}
	}

	void CopyCredentialCommand::Execute(IInspectable const& parameter)
	{
		Sarcophagus::MainVM mainVM = Sarcophagus::MainVM::GetInstance();

		if (auto credential = parameter.try_as<Sarcophagus::Credential>())
		{
			winrt::Windows::ApplicationModel::DataTransfer::DataPackage dataPackage{};
			dataPackage.SetText(credential.Password());
			winrt::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(dataPackage);
		}
	}
}

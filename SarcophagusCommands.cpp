#include "pch.h"
#include "SarcophagusCommands.h"

#if __has_include("EditCredentialCommand.g.cpp")
#include "EditCredentialCommand.g.cpp"
#endif
#if __has_include("CopyCredentialCommand.g.cpp")
#include "CopyCredentialCommand.g.cpp"
#endif

namespace winrt::Sarcophagus::implementation
{
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

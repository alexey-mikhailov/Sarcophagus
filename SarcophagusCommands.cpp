#include "pch.h"
#include "SarcophagusCommands.h"

#include "InternalCryptoTool.h"
#include "SarcophagusCommon.h"

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
		FileSerializer::GetInstance().MakeDirty();
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

		FileSerializer::GetInstance().MakeDirty();
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
			uint64_t pwdSize;
			uint8_t* pwdBuff;
			const ::Sarcophagus::InternalCryptoTool::DecryptResult result = ::Sarcophagus::InternalCryptoTool::GetInstance().Decrypt
			(
				sizeof(winrt::hstring::value_type) * credential.Password().size(),
				reinterpret_cast<const uint8_t*>(credential.Password().c_str()),
				&pwdSize,
				&pwdBuff
			);

			SARCOPHAGUS_ASSERT(result == ::Sarcophagus::InternalCryptoTool::DecryptResult::Success, NULL, L"Internal decryption failed. ");
			SARCOPHAGUS_ASSERT(pwdSize % 2 == 0, NULL, L"Decrypted password should be wide string (with even size). ");

			winrt::Windows::ApplicationModel::DataTransfer::DataPackage dataPackage{};
			dataPackage.SetText(winrt::hstring
			{
				reinterpret_cast<wchar_t*>(pwdBuff),
				static_cast<hstring::size_type>(pwdSize / 2)
			});
			delete pwdBuff;

			winrt::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(dataPackage);
		}
	}
}

#include "pch.h"
#include "SarcophagusCommands.h"

#include "cryptoengine_abstracts.h"
#include "cryptoselector.h"
#include "DialogTools.h"
#include "InternalCryptoTool.h"
#include "SarcophagusCommon.h"
#include "ViewModelHub.h"

#if __has_include("AddCredFolderCommand.g.cpp")
#include "AddCredFolderCommand.g.cpp"
#endif
#if __has_include("RemoveCredFolderCommand.g.cpp")
#include "RemoveCredFolderCommand.g.cpp"
#endif
#if __has_include("EditCredFolderCommand.g.cpp")
#include "EditCredFolderCommand.g.cpp"
#endif
#if __has_include("AddCredentialCommand.g.cpp")
#include "AddCredentialCommand.g.cpp"
#endif
#if __has_include("RemoveCredentialCommand.g.cpp")
#include "RemoveCredentialCommand.g.cpp"
#endif
#if __has_include("EditCredentialCommand.g.cpp")
#include "EditCredentialCommand.g.cpp"
#endif
#if __has_include("CopyCredentialCommand.g.cpp")
#include "CopyCredentialCommand.g.cpp"
#endif
#if __has_include("ChooseCryptoengineCommand.g.cpp")
#include "ChooseCryptoengineCommand.g.cpp"
#endif

namespace winrt::Sarcophagus::implementation
{
	void AddCredFolderCommand::Execute(IInspectable const&)
	{
		Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
		mainVM.CredFolders().Append(Sarcophagus::CredFolder(L"New folder"));
		FileSerializer::GetInstance().MakeDirty();
	}

	void RemoveCredFolderCommand::Execute(IInspectable const& parameter)
	{
		Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();

		if (auto credFolder = parameter.try_as<Sarcophagus::CredFolder>())
		{
			uint32_t index;
			if (mainVM.CredFolders().IndexOf(credFolder, index))
			{
				mainVM.CredFolders().RemoveAt(index);
			}
		}

		FileSerializer::GetInstance().MakeDirty();
	}

	void EditCredFolderCommand::Execute(IInspectable const& parameter)
	{
		Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();

		if (auto credFolder = parameter.try_as<Sarcophagus::CredFolder>())
		{
			mainVM.SelectedCredFolder(credFolder);
			mainVM.PageId(Sarcophagus::PageId::EditCredFolder);
		}
	}

	void AddCredentialCommand::Execute(IInspectable const&)
	{
		Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();

		if (mainVM.SelectedCredFolder())
		{
			mainVM.SelectedCredFolder().Credentials().Append(Sarcophagus::Credential(nullptr, L"New credential", ::Sarcophagus::EmptyString, ::Sarcophagus::EmptyString));
			FileSerializer::GetInstance().MakeDirty();
		}
		else
		{
			::Sarcophagus::ShowErrorAsync(L"Select folder first. ");
		}
	}

	void RemoveCredentialCommand::Execute(IInspectable const& parameter)
	{
		Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();

		if (auto credential = parameter.try_as<Sarcophagus::Credential>())
		{
			uint32_t index;
			if (mainVM.SelectedCredFolder().Credentials().IndexOf(credential, index))
			{
				mainVM.SelectedCredFolder().Credentials().RemoveAt(index);
			}
		}

		FileSerializer::GetInstance().MakeDirty();
	}

	void EditCredentialCommand::Execute(IInspectable const& parameter)
	{
		Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();

		if (auto credential = parameter.try_as<Sarcophagus::Credential>())
		{
			mainVM.CredentialTemplate(credential);
			mainVM.PageId(Sarcophagus::PageId::EditCredential);
		}
	}

	void CopyCredentialCommand::Execute(IInspectable const& parameter)
	{
		Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();

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

	void ChooseCryptoengineCommand::Execute(IInspectable const& parameter)
	{
		if (auto cryptoengine = parameter.try_as<Sarcophagus::CryptoengineVM>())
		{
			srfg::guid_t uuid;
			uuid.raw64[0] = cryptoengine.UuidPart1();
			uuid.raw64[1] = cryptoengine.UuidPart2();

			srfg::cryptoselector::get_instance().switch_engine(uuid);

			if (::Sarcophagus::ViewModelHub::GetInstance().ChooseCryptoengineVM().Target() == ChooseCryptoengineTarget::NewFile)
			{
				::Sarcophagus::ViewModelHub::GetInstance().MainVM().PageId(PageId::Main);
			}
			else if (::Sarcophagus::ViewModelHub::GetInstance().ChooseCryptoengineVM().Target() == ChooseCryptoengineTarget::SaveFile)
			{
				Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
				mainVM.SaveFileCommand().Execute(nullptr);
			}
			else if (::Sarcophagus::ViewModelHub::GetInstance().ChooseCryptoengineVM().Target() == ChooseCryptoengineTarget::SaveFileAs)
			{
				Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
				mainVM.SaveFileAsCommand().Execute(nullptr);
			}
		}
	}
}

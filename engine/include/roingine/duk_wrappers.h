#ifndef DUK_WRAPPERS_H
#define DUK_WRAPPERS_H

#include <optional>
#include <roingine/unique_duk_context.h>
#include <string_view>

struct duk_function_list_entry;
struct duk_number_list_entry;

namespace roingine {
	class FatalScriptError final : public std::exception {
	public:
		FatalScriptError(std::string errorMessage);

		char const *what() const noexcept override;

	private:
		std::string m_ErrorMessage;
	};

	class ScriptCompilationFailedException final : public std::exception {
	public:
		ScriptCompilationFailedException(std::string errorMessage);

		char const *what() const noexcept override;

	private:
		std::string m_ErrorMessage;
	};

	struct DukAddObjToParentInfo final {
		std::string key;
		int         parentIndex;
	};

	class DukObject final {
	public:
		DukObject(duk_context *ctx, std::optional<DukAddObjToParentInfo> addToParentInfo = std::nullopt);

		~DukObject();

		DukObject(DukObject &&);

		DukObject(DukObject const &) = delete;

		DukObject &operator=(DukObject &&);

		DukObject &operator=(DukObject const &) = delete;

		int GetInt(std::string_view key) const;

		void PutFunctionList(duk_function_list_entry const *functionList);

		void PutNumberList(duk_number_list_entry const *numberList);

		void MarkForReturn();

		[[nodiscard]]
		DukObject PushObject(std::string const &key) const;

		void PushPointer(std::string const &key, void *ptr) const;

	private:
		duk_context                         *m_DukContext;
		std::optional<DukAddObjToParentInfo> m_AddToParentInfo;
		int                                  m_StackBottomOffset;
		bool                                 m_ShouldPop{true};
	};

	class DukContext final {
	public:
		DukContext();

		~DukContext();

		DukContext(DukContext &&) = default;

		DukContext &operator=(DukContext &&) = default;

		DukContext(DukContext const &) = delete;

		DukContext &operator=(DukContext const &) = delete;

		[[nodiscard]]
		DukObject PushGlobalObject() const;

		void Eval(std::string_view code) const;

		[[nodiscard]]
		char const *GetGlobalString(std::string const &key) const;

		[[nodiscard]]
		duk_context *GetRawContext();

	private:
		UniqueDukContext m_DukContext;
	};

}// namespace roingine

#endif// DUK_WRAPPERS_H

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
		DukObject(duk_context *ctx, std::optional<DukAddObjToParentInfo> addToParentInfo);

		DukObject(duk_context *ctx);

		~DukObject();

		DukObject(DukObject &&);

		DukObject(DukObject const &) = delete;

		DukObject &operator=(DukObject &&);

		DukObject &operator=(DukObject const &) = delete;

		int GetInt(std::string_view key) const;

		void PutFunctionList(duk_function_list_entry const *functionList);

		void PutNumberList(duk_number_list_entry const *numberList);

		void MarkForReturn();

		DukObject PutObject(std::string const &key) const;

		void PutPointer(std::string const &key, void *ptr) const;

		[[nodiscard]]
		int GetStackIndex() const noexcept;

	private:
		duk_context                         *m_DukContext;
		std::optional<DukAddObjToParentInfo> m_AddToParentInfo{std::nullopt};
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
		DukObject AccessGlobalObject() const;

		[[nodiscard]]
		DukObject PushObject() const;

		void Eval(std::string_view code) const;

		[[nodiscard]]
		char const *GetGlobalString(std::string const &key) const;

		[[nodiscard]]
		duk_context *GetRawContext() const;

	private:
		UniqueDukContext m_DukContext;
	};

}// namespace roingine

#endif// DUK_WRAPPERS_H

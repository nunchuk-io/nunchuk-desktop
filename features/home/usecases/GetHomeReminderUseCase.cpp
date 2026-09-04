#include "GetHomeReminderUseCase.h"

#include "Servers/Draco.h"

namespace features::home::usecases {

core::usecase::Result<GetHomeReminderResult> GetHomeReminderUseCase::execute(const GetHomeReminderInput &input) {
    QJsonObject reminder;
    QString errorMessage;
    if (!Draco::instance()->GetHomeReminder(input.anonymous, reminder, errorMessage)) {
        return core::usecase::Result<GetHomeReminderResult>::failure(errorMessage);
    }
    GetHomeReminderResult output;
    output.reminder = reminder;
    return core::usecase::Result<GetHomeReminderResult>::success(std::move(output));
}

} // namespace features::home::usecases

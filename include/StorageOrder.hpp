#ifndef STORAGEORDER_HPP
#define STORAGEORDER_HPP

namespace algebra {

enum class StorageOrder {
    RowMajor,
    ColumnMajor
};

const char* storageOrderToString(StorageOrder order) {
    switch (order) {
        case StorageOrder::RowMajor: return "RowMajor";
        case StorageOrder::ColumnMajor: return "ColumnMajor";
        default: return "Unknown";
    }
}


} // namespace algebra

#endif //STORAGEORDER_HPP
#include "stdafx.h"
#include "cameramodel.h"
using namespace std;

CameraModel::CameraModel(int row, int col, QObject *parent)
	: QAbstractListModel(parent), m_row(0), m_col(0)
{
	initModel(row, col);
}

CameraModel::~CameraModel()
{

}

int CameraModel::rowCount(const QModelIndex& parent) const {
	return m_row * m_col;
}

QVariant CameraModel::data(const QModelIndex& index, int role) const {
	if (index.row() < 0 || index.row() >= m_row * m_col)
		return QVariant();
	if (role == BufferRole)
		return m_buffer.at(index.row());
	else if (role == SelectedRole)
		return m_selected.at(index.row());

	return QVariant();
}

bool CameraModel::setData(const QModelIndex & index, const QVariant & value, int role) {
	if (role == SelectedRole) {
		m_selected[index.row()] = value.toBool();
		emit dataChanged(index, index, { SelectedRole });
		return true;
	}
	return false;
}

Qt::ItemFlags CameraModel::flags(const QModelIndex& index) const {
	return Qt::ItemIsEditable;
}

QHash<int, QByteArray> CameraModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[BufferRole] = "buffer";
	roles[SelectedRole] = "selected";
	return roles;
}

int CameraModel::rows() const {
	return m_row;
}

int CameraModel::cols() const {
	return m_col;
}

QPoint CameraModel::selectedCell() const {
	auto p = find(begin(m_selected), end(m_selected), true);
	return indexToPoint(p - begin(m_selected));
}

void CameraModel::initModel(int row, int col) {
	auto index = this->createIndex(0, 0);
	beginRemoveRows(index, 0, rowCount());
	m_buffer.erase(begin(m_buffer), end(m_buffer));
	m_selected.erase(begin(m_selected), end(m_selected));
	m_hasImage.erase(begin(m_hasImage), end(m_hasImage));
	endRemoveRows();
	
	beginInsertRows(index, 0, row * col);
	m_selected = vector<bool>(row * col);
	fill(begin(m_selected), end(m_selected), false);
	m_hasImage = vector<bool>(row * col);
	fill(begin(m_hasImage), end(m_hasImage), false);
	m_buffer = vector<QImage>(row * col);
	fill(begin(m_buffer), end(m_buffer), QImage(10, 10, QImage::Format_RGB888));
	endInsertRows();

	m_row = row;
	emit rowsChanged(m_row);
	m_col = col;
	emit colsChanged(m_col);
}

void CameraModel::clearBuffers() {
	fill(begin(m_buffer), end(m_buffer), QImage(10, 10, QImage::Format_RGB888));
	auto tl = createIndex(0, 0);
	auto br = createIndex(rowCount() - 1, 0);
	emit dataChanged(tl, br, { BufferRole });
}

void CameraModel::clearBuffersAt(const QPoint& target) {
	auto index = pointToIndex(target);
	m_buffer[index] = QImage(10, 10, QImage::Format_RGB888);
	auto mi = createIndex(index, 0);
	emit dataChanged(mi, mi, { BufferRole });
}

void CameraModel::updateBuffer(const QImage& buffer, const QPoint& target) {
	int index = target.x() + target.y() * m_row;
	if (index >= 0 && index < rowCount()) {
		m_buffer[index] = buffer;
		m_hasImage[index] = true;
		auto modelIndex = createIndex(index, 0);
		emit dataChanged(modelIndex, modelIndex, { BufferRole });
	}
}

void CameraModel::saveBuffers(const QString& baseDir) {
	// We'll implement this later
}

void CameraModel::clearSelection() {
	fill(begin(m_selected), end(m_selected), false);
	auto tl = createIndex(0, 0);
	auto br = createIndex(rowCount() - 1, 0);
	emit dataChanged(tl, br, { SelectedRole });
}

void CameraModel::select(const QPoint& target) {
	auto index = pointToIndex(target);
	m_selected[index] = true;
	auto mi = createIndex(index, 0);
	emit dataChanged(mi, mi, { SelectedRole });
}

QPoint CameraModel::indexToPoint(int index) const {
	int col = index % m_row;
	int row = (index - col) / m_col;
	return QPoint(col, row);
}

int CameraModel::pointToIndex(const QPoint& point) const {
	return point.x() + point.y() * m_row;
}

std::vector<QPoint> CameraModel::autoFill() const {
	// Find top-left and bottom-right filled cell
	auto first_i = find(begin(m_hasImage), end(m_hasImage), true);
	int first = distance(begin(m_hasImage), first_i);
	auto last_i = find(rbegin(m_hasImage), rend(m_hasImage), true);
	int last = distance(begin(m_hasImage), last_i.base());

	// Calculate cells in the box
	auto tl = indexToPoint(first);
	auto br = indexToPoint(last);
	std::vector<QPoint> res;
	for (int y = tl.y(); y <= br.y(); ++y) {
		for (int x = tl.x(); x <= br.x(); ++x) {
			if (!m_hasImage[pointToIndex(QPoint(x, y))]) {
				res.push_back(QPoint(x, y));
			}
		}
	}

	return res;
}

std::vector<QPoint> CameraModel::boxFill() const {
	// Selection is already restricted to box area by UI,
	// thus we simply return selected cells with empty image
	std::vector<QPoint> res;
	for (size_t i = 0; i < m_selected.size(); ++i) {
		if (m_selected.at(i) && !m_hasImage.at(i))
			res.push_back(indexToPoint(i));
	}
	return res;
}
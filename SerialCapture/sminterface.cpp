#include "stdafx.h"
#include "sminterface.h"
using namespace std;

SMInterface::SMInterface(int profileIndex, QObject *parent)
	: QObject(parent), m_overlap(.25), m_rows(0), m_cols(0), m_grid(0)
{
	loadProfile(profileIndex);
}

SMInterface::~SMInterface()
{

}

class GenPointF
{
	int m_col, x;
	int y;
	double w, h;
public:
	GenPointF(int col, const QSizeF &sz) :
		m_col(col), x(0), y(0), w(sz.width()), h(sz.height()) { }
	QPointF operator()() {
		QPointF res{ 1.0*x*w, 1.0*y*h };
		++x;
		if (x == m_col) {
			x = 0;
			++y;
		}
		return res;
	}
};

void SMInterface::loadProfile(int profile) {
	m_profile = profile;
	// Initialize necessary data
	QSettings s{ "Miconos", "Optilab", this };
	QString name = QString{ "PROFILE_%1" }.arg(profile);
	auto sz = s.value(name, QSizeF(10000.0, 10000.0)).toSizeF();
	m_size = QSizeF((1.0 - m_overlap) * sz.width() / 1000.0, (1.0 - m_overlap) * sz.height() / 1000.0);
	double xlim = s.value("X_LIMIT", 100).toDouble();
	double ylim = s.value("Y_LIMIT", 100).toDouble();
	m_cols = xlim / m_size.width();		// Limit is stored in mm,
	m_rows = ylim / m_size.height();	// Calibration is stored in um
	
	// Generate coordinate table
	m_grid.erase(begin(m_grid), end(m_grid));
	m_grid = vector<QPointF>(m_cols*m_rows);
	GenPointF gen{ m_cols, m_size };
	generate(begin(m_grid), end(m_grid), gen);

	// Align stepper to nearest cell
	alignToGrid();
}

void SMInterface::alignToGrid() {
	auto index = coordToIndex(m_pos);
	moveTo(index);
}

QPoint SMInterface::coordToIndex(const QPointF& pos) const {
	auto np = pos;
	auto comp = [=](const QPointF& val) -> bool {
		if (fabs(np.x() - val.x()) <= .00001 &&
			fabs(np.y() - val.y()) <= .00001)
			return true;
		return false;
	};
	auto found = find_if(begin(m_grid), end(m_grid), comp);
	if (found == end(m_grid))
		return m_index;
	int index = distance(begin(m_grid), found);
	int col = index % m_rows;
	int row = (index - col) / m_cols;
	return QPoint(col, row);
}

QPointF SMInterface::indexToCoord(const QPoint& pos) const {
	int index = pos.x() + pos.y() * m_rows;
	return m_grid.at(index);
}

void SMInterface::updatePos(const QPointF &pos) {
	if (pos != m_pos) {
		m_pos = pos;
		if (coordToIndex(m_pos) != m_index) {
			m_index = coordToIndex(m_pos);
			emit cellPosChanged(m_index);
		}
	}
}

void SMInterface::moveTo(const QPoint& index) {
	emit stepperMoveTo(indexToCoord(index));
}

void SMInterface::setOverlap(double& ovl) { 
	if (m_overlap != ovl) {
		m_overlap = ovl;
		loadProfile(m_profile);
	}
}